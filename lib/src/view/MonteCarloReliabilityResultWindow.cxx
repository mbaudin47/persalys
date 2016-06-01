//                                               -*- C++ -*-
/**
 *  @brief QMdiSubWindow for the results of the reliability analysis by Monte Carlo
 *
 *  Copyright 2015-2016 EDF-Phimeca
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
#include "otgui/CustomStandardItemModel.hxx"
#include "otgui/ResizableTableViewWithoutScrollBar.hxx"

#include <qwt_legend.h>
#include <qwt_scale_engine.h>

#include <QVBoxLayout>
#include <QLabel>
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
  outputNameLabel = new QLabel(QString::fromUtf8(result_.getSimulationResult().getEvent().getDescription()[0].c_str()));
  headLayout->addWidget(outputNameLabel);
  headLayout->addStretch();
  tabLayout->addLayout(headLayout);

  // number of simulations
  QLabel * nbSimuLabel = new QLabel(tr("Number of simulations:") + " " + QString::number(result_.getSimulationResult().getOuterSampling()*result_.getSimulationResult().getBlockSize()) + "\n");
  nbSimuLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
  tabLayout->addWidget(nbSimuLabel);

  // probability estimate table
  ResizableTableViewWithoutScrollBar * resultsTable = new ResizableTableViewWithoutScrollBar;
  resultsTable->horizontalHeader()->hide();
  resultsTable->verticalHeader()->hide();
  CustomStandardItemModel * resultsTableModel = new CustomStandardItemModel(4, 4);
  resultsTable->setModel(resultsTableModel);

  // horizontal header
  resultsTableModel->setNotEditableHeaderItem(0, 0, tr("Estimate"));
  resultsTable->setSpan(0, 0, 2, 1);

  resultsTableModel->setNotEditableHeaderItem(0, 1, tr("Value"));
  resultsTable->setSpan(0, 1, 2, 1);

  // Failure probability
  resultsTableModel->setNotEditableHeaderItem(2, 0, tr("Failure probability"));
  resultsTableModel->setNotEditableItem(2, 1, result_.getSimulationResult().getProbabilityEstimate());

  // Coefficient of variation
  resultsTableModel->setNotEditableHeaderItem(3, 0, tr("Coefficient of variation"));
  resultsTableModel->setNotEditableItem(3, 1, result_.getSimulationResult().getCoefficientOfVariation());

  // - lower bound
  resultsTableModel->setNotEditableHeaderItem(1, 2, tr("Lower bound"));
  double pfCILowerBound = std::max(0.0, result_.getSimulationResult().getProbabilityEstimate() - 0.5 * result_.getSimulationResult().getConfidenceLength());
  resultsTableModel->setNotEditableItem(2, 2, pfCILowerBound);

  // - upper bound
  resultsTableModel->setNotEditableHeaderItem(1, 3, tr("Upper bound"));
  double pfCIUpperBound = std::min(1.0, result_.getSimulationResult().getProbabilityEstimate() + 0.5 * result_.getSimulationResult().getConfidenceLength());
  resultsTableModel->setNotEditableItem(2, 3, pfCIUpperBound);

  resultsTable->resizeToContents();
  
  // Confidence interval: do it after resizeToContents
  resultsTableModel->setNotEditableHeaderItem(0, 2, tr("Confidence interval at 95%"));
  resultsTable->setSpan(0, 2, 1, 2);

  tabLayout->addWidget(resultsTable);
  tabLayout->addStretch();

  tabWidget_->addTab(tab, tr("Summary"));

  // second tab --------------------------------
  tab = new QWidget;
  tabLayout = new QVBoxLayout(tab);

  // output histogram
  QVector<PlotWidget*> listHistogram;
  PlotWidget * plot = new PlotWidget;
  QString outputName(QString::fromUtf8(result_.getSimulationResult().getEvent().getDescription()[0].c_str()));
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
#if (QWT_VERSION >= 0x060100)
  QwtLogScaleEngine * scaleEngin = new QwtLogScaleEngine();
#else
  QwtScaleEngine * scaleEngin = new QwtLog10ScaleEngine();
#endif
  plot->setAxisScaleEngine(QwtPlot::xBottom, scaleEngin);

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
  else if (newState == 0 || newState == 1 || newState == 9)
    showHideGraphConfigurationWidget(-1);
}
}