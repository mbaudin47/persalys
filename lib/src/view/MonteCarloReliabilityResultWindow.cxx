//                                               -*- C++ -*-
/**
 *  @brief QMdiSubWindow for the results of the reliability analysis by Monte Carlo
 *
 *  Copyright 2015-2016 EDF
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "otgui/MonteCarloReliabilityResultWindow.hxx"

#include "otgui/MonteCarloReliabilityAnalysis.hxx"

#include <qwt_legend.h>

#include <QVBoxLayout>
#include <QLabel>
#include <QTabWidget>
#include <QTableWidget>
#include <QHeaderView>

using namespace OT;

namespace OTGUI {

MonteCarloReliabilityResultWindow::MonteCarloReliabilityResultWindow(AnalysisItem * item)
  : OTguiSubWindow(item)
  , result_(dynamic_cast<MonteCarloReliabilityAnalysis*>(&*item->getAnalysis().getImplementation())->getResult())
{
  buildInterface();
  connect(this, SIGNAL(windowStateChanged(Qt::WindowStates, Qt::WindowStates)), this, SLOT(showHideGraphConfigurationWidget(Qt::WindowStates, Qt::WindowStates)));
}


void MonteCarloReliabilityResultWindow::buildInterface()
{
  tabWidget_ = new QTabWidget;

  // first tab --------------------------------
  QWidget * tab = new QWidget;
  QVBoxLayout * tabLayout = new QVBoxLayout(tab);

  QHBoxLayout * headLayout = new QHBoxLayout;
  QLabel * outputNameLabel = new QLabel(tr("Output"));
  headLayout->addWidget(outputNameLabel);
  outputNameLabel = new QLabel(result_.getSimulationResult().getEvent().getDescription()[0].c_str());
  headLayout->addWidget(outputNameLabel);
  headLayout->addStretch();
  tabLayout->addLayout(headLayout);

  // number of simulations
  QLabel * nbSimuLabel = new QLabel(tr("Number of simulations : ") + QString::number(result_.getSimulationResult().getOuterSampling()*result_.getSimulationResult().getBlockSize()) + "\n");
  nbSimuLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
  tabLayout->addWidget(nbSimuLabel);

  // probability estimate table
  QTableWidget * resultsTable_ = new QTableWidget(4, 4);
  resultsTable_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  resultsTable_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  resultsTable_->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
  resultsTable_->verticalHeader()->hide();
  resultsTable_->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
  resultsTable_->horizontalHeader()->hide();

  QTableWidgetItem * item = new QTableWidgetItem("Estimate");
  item->setFlags(item->flags() ^ Qt::ItemIsEditable);
  item->setBackgroundColor(resultsTable_->verticalHeader()->palette().color(QPalette::Active, QPalette::Background));
  item->setTextAlignment(Qt::AlignCenter);
  resultsTable_->setSpan(0, 0, 2, 1);
  resultsTable_->setItem(0, 0, item);

  item = new QTableWidgetItem(tr("Value"));
  item->setFlags(item->flags() ^ Qt::ItemIsEditable);
  item->setBackgroundColor(resultsTable_->verticalHeader()->palette().color(QPalette::Active, QPalette::Background));
  item->setTextAlignment(Qt::AlignCenter);
  resultsTable_->setSpan(0, 1, 2, 1);
  resultsTable_->setItem(0, 1, item);

  // Failure probability
  item = new QTableWidgetItem(tr("Failure probability"));
  item->setFlags(item->flags() ^ Qt::ItemIsEditable);
  item->setBackgroundColor(resultsTable_->verticalHeader()->palette().color(QPalette::Active, QPalette::Background));
  resultsTable_->setItem(2, 0, item);

  item = new QTableWidgetItem(QString::number(result_.getSimulationResult().getProbabilityEstimate()));
  item->setFlags(item->flags() ^ Qt::ItemIsEditable);
  resultsTable_->setItem(2, 1, item);

  // Coefficient of variation
  item = new QTableWidgetItem(tr("Coefficient of variation"));
  item->setFlags(item->flags() ^ Qt::ItemIsEditable);
  item->setBackgroundColor(resultsTable_->verticalHeader()->palette().color(QPalette::Active, QPalette::Background));
  resultsTable_->setItem(3, 0, item);
  resultsTable_->resizeColumnToContents(0);

  item = new QTableWidgetItem(QString::number(result_.getSimulationResult().getCoefficientOfVariation()));
  item->setFlags(item->flags() ^ Qt::ItemIsEditable);
  resultsTable_->setItem(3, 1, item);

  // Confidence interval
  item = new QTableWidgetItem(tr("Confidence interval at 95%"));
  item->setFlags(item->flags() ^ Qt::ItemIsEditable);
  item->setBackgroundColor(resultsTable_->verticalHeader()->palette().color(QPalette::Active, QPalette::Background));
  item->setTextAlignment(Qt::AlignCenter);
  resultsTable_->setSpan(0, 2, 1, 2);
  resultsTable_->setItem(0, 2, item);

  // - lower bound
  item = new QTableWidgetItem(tr("Lower bound"));
  item->setFlags(item->flags() ^ Qt::ItemIsEditable);
  item->setBackgroundColor(resultsTable_->verticalHeader()->palette().color(QPalette::Active, QPalette::Background));
  item->setTextAlignment(Qt::AlignCenter);
  resultsTable_->setItem(1, 2, item);

  double pfCILowerBound = std::max(0.0, result_.getSimulationResult().getProbabilityEstimate() - 0.5 * result_.getSimulationResult().getConfidenceLength());
  item = new QTableWidgetItem(QString::number(pfCILowerBound));
  item->setFlags(item->flags() ^ Qt::ItemIsEditable);
  resultsTable_->setItem(2, 2, item);

  // - upper bound
  item = new QTableWidgetItem(tr("Upper bound"));
  item->setFlags(item->flags() ^ Qt::ItemIsEditable);
  item->setBackgroundColor(resultsTable_->verticalHeader()->palette().color(QPalette::Active, QPalette::Background));
  item->setTextAlignment(Qt::AlignCenter);
  resultsTable_->setItem(1, 3, item);

  double pfCIUpperBound = std::min(1.0, result_.getSimulationResult().getProbabilityEstimate() + 0.5 * result_.getSimulationResult().getConfidenceLength());
  item = new QTableWidgetItem(QString::number(pfCIUpperBound));
  item->setFlags(item->flags() ^ Qt::ItemIsEditable);
  resultsTable_->setItem(2, 3, item);

  QSize size = resultsTable_->sizeHint();
  int width = 0;
  for (int i=0; i<resultsTable_->columnCount(); ++i)
    width += resultsTable_->columnWidth(i);
  size.setWidth(width);
  int height = 0;
  for (int i=0; i<resultsTable_->rowCount(); ++i)
    height += resultsTable_->rowHeight(i);
  size.setHeight(height);
  resultsTable_->setMinimumSize(size);
  resultsTable_->setMaximumSize(size);
  resultsTable_->updateGeometry();

  tabLayout->addWidget(resultsTable_);
  tabLayout->addStretch();

  tabWidget_->addTab(tab, tr("Summary"));

  // second tab --------------------------------
  tab = new QWidget;
  tabLayout = new QVBoxLayout(tab);

  // output histogram
  QVector<PlotWidget*> listHistogram;
  PlotWidget * plot = new PlotWidget;
  QString outputName(result_.getSimulationResult().getEvent().getDescription()[0].c_str());
  plot->plotHistogram(result_.getOutputSample(), 2, 0, outputName + tr(" distribution"));
  NumericalSample threshold = NumericalSample(2, 2);
  threshold[0][0] = result_.getSimulationResult().getEvent().getThreshold();
  threshold[1][0] = plot->axisInterval(QwtPlot::yLeft).minValue();
  threshold[1][0] = result_.getSimulationResult().getEvent().getThreshold();
  threshold[1][1] = plot->axisInterval(QwtPlot::yLeft).maxValue();
  plot->plotCurve(threshold, QPen(Qt::red), QwtPlotCurve::Lines, 0, tr("Threshold"));
  plot->setAxisTitle(QwtPlot::xBottom, tr("Values"));
  plot->setAxisTitle(QwtPlot::yLeft, tr("Number of simulations"));
  plot->insertLegend(new QwtLegend(), QwtPlot::BottomLegend);
  plot->setTitle(tr("Output ") + outputName + tr(" distribution"));

  listHistogram.append(plot);
  tabLayout->addWidget(plot);

  histogramConfigurationWidget_ = new GraphConfigurationWidget(listHistogram, QStringList(), QStringList()<<outputName, GraphConfigurationWidget::NoType);

  tabWidget_->addTab(tab, tr("Histogram"));

  // third tab --------------------------------
  tab = new QWidget;
  tabLayout = new QVBoxLayout(tab);

  QVector<PlotWidget*> listConvergenceGraph;
  plot = new PlotWidget;
  plot->plotCurve(result_.getConvergenceSample(), QPen(Qt::red), QwtPlotCurve::Lines, 0, tr("Probability estimate"));
  if (result_.getConvergenceSampleLowerBound().getSize())
    plot->plotCurve(result_.getConvergenceSampleLowerBound(), QPen(Qt::green), QwtPlotCurve::Lines, 0, tr("Lower bound"));
  if (result_.getConvergenceSampleUpperBound().getSize())
    plot->plotCurve(result_.getConvergenceSampleUpperBound(), QPen(Qt::green), QwtPlotCurve::Lines, 0, tr("Upper bound"));
  plot->setTitle(tr("Monte Carlo convergence graph at level 0.95"));
  plot->setAxisTitle(QwtPlot::yLeft, tr("Estimate"));
  plot->setAxisTitle(QwtPlot::xBottom, tr("Outer iteration"));
  plot->insertLegend(new QwtLegend(), QwtPlot::BottomLegend);

  listConvergenceGraph.append(plot);
  tabLayout->addWidget(plot);

  convergenceGraphConfigurationWidget_ = new GraphConfigurationWidget(listConvergenceGraph, QStringList(), QStringList()<<outputName, GraphConfigurationWidget::NoType);

  tabWidget_->addTab(tab, tr("Convergence graph"));

  //
  connect(tabWidget_, SIGNAL(currentChanged(int)), this, SLOT(showHideGraphConfigurationWidget(int)));
  setWidget(tabWidget_);
}


void MonteCarloReliabilityResultWindow::showHideGraphConfigurationWidget(int indexTab)
{
  switch (indexTab)
  {
    // if a plotWidget is visible
    case 1:
      emit graphWindowActivated(histogramConfigurationWidget_);
      break;
    case 2:
      emit graphWindowActivated(convergenceGraphConfigurationWidget_);
      break;
    // if no plotWidget is visible
    default:
    {
      emit graphWindowDeactivated(histogramConfigurationWidget_);
      emit graphWindowDeactivated(convergenceGraphConfigurationWidget_);
      break;
    }
  }
}


void MonteCarloReliabilityResultWindow::showHideGraphConfigurationWidget(Qt::WindowStates oldState, Qt::WindowStates newState)
{
  if (newState == 4 || newState == 8 || newState == 10)
    showHideGraphConfigurationWidget(tabWidget_->currentIndex());
  else if (newState == 0 || newState == 1 || newState == 2 || newState == 9)
    showHideGraphConfigurationWidget(-1);
}
}