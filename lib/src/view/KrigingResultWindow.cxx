//                                               -*- C++ -*-
/**
 *  @brief QMdiSubWindow for the results of functional chaos
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
#include "otgui/KrigingResultWindow.hxx"

#include "otgui/KrigingAnalysis.hxx"
#include "otgui/FunctionalChaosResultWindow.hxx"
#include "otgui/ResizableStackedWidget.hxx"
#include "otgui/ResizableTableViewWithoutScrollBar.hxx"
#include "otgui/MetaModelValidationWidget.hxx"

#include "openturns/OTBase.hxx"

#include <QVBoxLayout>
#include <QGroupBox>
#include <QScrollArea>
#include <QSplitter>

using namespace OT;

namespace OTGUI {

KrigingResultWindow::KrigingResultWindow(AnalysisItem * item)
  : ResultWindow(item)
  , result_(dynamic_cast<KrigingAnalysis*>(&*item->getAnalysis().getImplementation())->getResult())
  , outputsListWidget_(0)
  , tabWidget_(0)
{
  setParameters(item->getAnalysis());
  buildInterface();
}


void KrigingResultWindow::setParameters(const Analysis& analysis)
{
  const KrigingAnalysis kriging = *dynamic_cast<const KrigingAnalysis*>(&*analysis.getImplementation());

  // ParametersWidget
  QStringList namesList;
  namesList << tr("Algorithm");
  namesList << tr("Correlation model");
  namesList << tr("Scale");
  namesList << tr("Amplitude");
  if (kriging.getCovarianceModel().getImplementation()->getClassName() == "MaternModel")
    namesList << tr("nu");
  else if (kriging.getCovarianceModel().getImplementation()->getClassName() == "GeneralizedExponential")
    namesList << tr("p");
  namesList << tr("Trend basis");
  namesList << tr("Leave-one-out validation");

  QStringList valuesList;
  valuesList << tr("Kriging");
  valuesList << QString(kriging.getCovarianceModel().getImplementation()->getClassName().c_str());
  // scale
  QString scaleText;
  for (UnsignedInteger i=0; i<kriging.getCovarianceModel().getScale().getSize(); ++i)
  {
    scaleText += QString::number(kriging.getCovarianceModel().getScale()[i]);
    if (i < kriging.getCovarianceModel().getScale().getSize()-1)
      scaleText += "; ";
  }
  valuesList << scaleText;
  // amplitude
  QString amplitudeText;
  for (UnsignedInteger i=0; i<kriging.getCovarianceModel().getAmplitude().getSize(); ++i)
  {
    amplitudeText += QString::number(kriging.getCovarianceModel().getAmplitude()[i]);
    if (i < kriging.getCovarianceModel().getAmplitude().getSize()-1)
      amplitudeText += "; ";
  }
  valuesList << amplitudeText;
  // covariance model parameters
  if (kriging.getCovarianceModel().getImplementation()->getClassName() == "MaternModel")
  {
    double nu = dynamic_cast<MaternModel*>(&*kriging.getCovarianceModel().getImplementation())->getNu();
    valuesList << QString::number(nu);
  }
  else if (kriging.getCovarianceModel().getImplementation()->getClassName() == "GeneralizedExponential")
  {
    double p = dynamic_cast<GeneralizedExponential*>(&*kriging.getCovarianceModel().getImplementation())->getP();
    valuesList << QString::number(p);
  }
  // basis
  QString basisType(tr("Constant"));
  const UnsignedInteger dim = kriging.getBasis().getDimension();
  if (kriging.getBasis().getSize() == (dim+1))
    basisType = tr("Linear");
  else if (kriging.getBasis().getSize() == ((dim+1)*(dim+2)/2))
    basisType = tr("Quadratic");
  valuesList << basisType;

  valuesList << (kriging.isLeaveOneOutValidation()? tr("yes") : tr("no"));

  parametersWidget_ = new ParametersWidget(tr("Metamodel creation parameters:"), namesList, valuesList);
}


void KrigingResultWindow::buildInterface()
{
  setWindowTitle(tr("Kriging results"));

  // get output info
  const UnsignedInteger outputDimension = result_.getOutputSample().getDimension();
  QStringList outputNames;
  for (UnsignedInteger i=0; i<outputDimension; ++i)
    outputNames << QString::fromUtf8(result_.getOutputSample().getDescription()[i].c_str());

  // main splitter
  QSplitter * mainWidget = new QSplitter(Qt::Horizontal);
  tabWidget_ = new QTabWidget;

  // list outputs
  QGroupBox * outputsGroupBox = new QGroupBox(tr("Outputs"));
  QVBoxLayout * outputsLayoutGroupBox = new QVBoxLayout(outputsGroupBox);

  outputsListWidget_ = new QListWidget;
  outputsListWidget_->addItems(outputNames);
  outputsLayoutGroupBox->addWidget(outputsListWidget_);
  outputsLayoutGroupBox->addStretch();

  mainWidget->addWidget(outputsGroupBox);
  mainWidget->setStretchFactor(0, 1);

  // tab widget
  tabWidget_ = new QTabWidget;

  // first tab : METAMODEL GRAPH --------------------------------
  QWidget * tab = new QWidget;
  QVBoxLayout * metaModelPlotLayout = new QVBoxLayout(tab);

  ResizableStackedWidget * plotsStackedWidget = new ResizableStackedWidget;
  connect(outputsListWidget_, SIGNAL(currentRowChanged(int)), plotsStackedWidget, SLOT(setCurrentIndex(int)));
  for (UnsignedInteger i=0; i<outputDimension; ++i)
  {
    MetaModelValidationWidget * validationWidget = new MetaModelValidationWidget(result_.getMetaModelOutputSample().getMarginal(i),
                                                                                 result_.getOutputSample().getMarginal(i),
                                                                                 result_.getKrigingResultCollection()[i].getResiduals()[0],
                                                                                 result_.getKrigingResultCollection()[i].getRelativeErrors()[0],
                                                                                 true,
                                                                                 i);

    plotsStackedWidget->addWidget(validationWidget);
    connect(validationWidget, SIGNAL(graphWindowActivated(QWidget*)), this, SIGNAL(graphWindowActivated(QWidget*)));
    connect(outputsListWidget_, SIGNAL(currentRowChanged(int)), validationWidget, SLOT(showHideGraphConfigurationWidget(int)));
    connect(this, SIGNAL(stateChanged(int)), validationWidget, SLOT(showHideGraphConfigurationWidget(int)));
  }
  metaModelPlotLayout->addWidget(plotsStackedWidget);

  tabWidget_->addTab(tab, tr("Metamodel"));

  // second tab : GRAPH METAMODEL LOO --------------------------------
  if (result_.getMetaModelOutputSampleLeaveOneOut().getSize())
  {
    QTabWidget * validationTabWidget = new QTabWidget;

    tab = new QWidget;
    metaModelPlotLayout = new QVBoxLayout(tab);

    ResizableStackedWidget * plotsLOOStackedWidget = new ResizableStackedWidget;
    connect(outputsListWidget_, SIGNAL(currentRowChanged(int)), plotsLOOStackedWidget, SLOT(setCurrentIndex(int)));
    for (UnsignedInteger i=0; i<outputDimension; ++i)
    {
      MetaModelValidationWidget * validationWidget = new MetaModelValidationWidget(result_.getMetaModelOutputSampleLeaveOneOut().getMarginal(i),
                                                                                   result_.getOutputSample().getMarginal(i),
                                                                                   result_.getErrorQ2LeaveOneOut()[i],
                                                                                   result_.getQ2LeaveOneOut()[i],
                                                                                   false,
                                                                                   i);
      plotsLOOStackedWidget->addWidget(validationWidget);
  
      connect(validationWidget, SIGNAL(graphWindowActivated(QWidget*)), this, SIGNAL(graphWindowActivated(QWidget*)));
      connect(outputsListWidget_, SIGNAL(currentRowChanged(int)), validationWidget, SLOT(showHideGraphConfigurationWidget(int)));
      connect(this, SIGNAL(stateChanged(int)), validationWidget, SLOT(showHideGraphConfigurationWidget(int)));
    }
    metaModelPlotLayout->addWidget(plotsLOOStackedWidget);

    validationTabWidget->addTab(plotsLOOStackedWidget, tr("Leave-one-out"));
    tabWidget_->addTab(validationTabWidget, tr("Validation"));
  }

  // third tab : PARAMETERS --------------------------------
  if (parametersWidget_)
    tabWidget_->addTab(parametersWidget_, tr("Parameters"));

  // set widgets
  connect(tabWidget_, SIGNAL(currentChanged(int)), this, SLOT(showHideGraphConfigurationWidget(int)));

  mainWidget->addWidget(tabWidget_);
  mainWidget->setStretchFactor(1, 10);
  outputsListWidget_->setCurrentRow(0);
  setWidget(mainWidget);
}


void KrigingResultWindow::showHideGraphConfigurationWidget(int indexTab)
{

  if (indexTab == 0 || // metamodel
      indexTab == 1    // validation
     )
    emit stateChanged(outputsListWidget_->currentRow());
  else
    emit graphWindowDeactivated();
}


void KrigingResultWindow::showHideGraphConfigurationWidget(Qt::WindowStates oldState, Qt::WindowStates newState)
{
  if (oldState == Qt::WindowMaximized)
    return;

  if (newState == Qt::WindowFullScreen || newState == (Qt::WindowActive|Qt::WindowMaximized))
    showHideGraphConfigurationWidget(tabWidget_->currentIndex());
  else if (newState == Qt::WindowNoState || newState == Qt::WindowMinimized || newState == (Qt::WindowActive|Qt::WindowMinimized))
    showHideGraphConfigurationWidget(-1);
}
}