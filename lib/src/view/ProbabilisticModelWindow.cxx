//                                               -*- C++ -*-
/**
 *  @brief QMdiSubWindow to define a probabilistic model
 *
 *  Copyright 2015-2017 EDF-Phimeca
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
#include "otgui/ProbabilisticModelWindow.hxx"

#include "otgui/ComboBoxDelegate.hxx"
#include "otgui/SpinBoxDelegate.hxx"
#include "otgui/InputTableProbabilisticModel.hxx"
#include "otgui/DistributionDictionary.hxx"
#include "otgui/CorrelationTableModel.hxx"
#include "otgui/CollapsibleGroupBox.hxx"
#include "otgui/QtTools.hxx"
#include "otgui/InferenceResultWizard.hxx"
#include "otgui/InferenceAnalysis.hxx"

#include "openturns/Normal.hxx"
#include "openturns/TruncatedDistribution.hxx"
#include "openturns/TruncatedNormal.hxx"

#include <QSplitter>
#include <QScrollArea>
#include <QPushButton>
#include <QStackedWidget>
#include <QMessageBox>

using namespace OT;

namespace OTGUI {

ProbabilisticModelWindow::ProbabilisticModelWindow(const OTStudy& otStudy, ProbabilisticModelItem * item)
  : OTguiSubWindow(item)
  , otStudy_(otStudy)
  , physicalModel_(item->getPhysicalModel())
  , pdf_cdfPlotsConfigurationWidget_(0)
  , paramEditor_(0)
{
  connect(item, SIGNAL(inputChanged()), this, SLOT(updateProbabilisticModel()));
  buildInterface();
  connect(this, SIGNAL(windowStateChanged(Qt::WindowStates, Qt::WindowStates)), this, SLOT(showHideGraphConfigurationWidget(Qt::WindowStates, Qt::WindowStates)));
}


ProbabilisticModelWindow::~ProbabilisticModelWindow()
{
  delete pdf_cdfPlotsConfigurationWidget_;
  pdf_cdfPlotsConfigurationWidget_ = 0;
}


void ProbabilisticModelWindow::buildInterface()
{
  setWindowTitle(tr("Probabilistic model"));

  QTabWidget * rootTab = new QTabWidget;
  
  // First Tab: marginals
  QWidget * tab = new QWidget;
  QVBoxLayout * tabLayout = new QVBoxLayout(tab);

  QSplitter * horizontalSplitter = new QSplitter;

  // Inputs table
  inputTableView_ = new QTableView;
  inputTableView_->setSelectionMode(QAbstractItemView::SingleSelection);
  inputTableHeaderView_ = new CheckableHeaderView;
  if (physicalModel_.hasStochasticInputs() && (physicalModel_.getComposedDistribution().getDimension() == physicalModel_.getInputs().getSize()))
    inputTableHeaderView_->setChecked(true);
  inputTableView_->setHorizontalHeader(inputTableHeaderView_);
  QStringList items;
  Description listDistributions = DistributionDictionary::GetAvailableDistributions();
  for (UnsignedInteger i=0; i<listDistributions.getSize(); ++i)
    items << tr(listDistributions[i].c_str());
  items << tr("Inference result");

  ComboBoxDelegate * delegate = new ComboBoxDelegate(items);
  inputTableView_->setItemDelegateForColumn(1, delegate);
#if QT_VERSION >= 0x050000
  inputTableView_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#else
  inputTableView_->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#endif
  inputTableView_->setSelectionBehavior(QAbstractItemView::SelectRows);

  inputTableModel_ = new InputTableProbabilisticModel(physicalModel_, inputTableView_);
  inputTableView_->setModel(inputTableModel_);
  inputTableView_->resizeColumnToContents(0);

  for (int i = 0; i < inputTableModel_->rowCount(); ++i)
    inputTableView_->openPersistentEditor(inputTableModel_->index(i, 1));

  connect(inputTableView_, SIGNAL(clicked(QModelIndex)), this, SLOT(updateDistributionWidgets(const QModelIndex&)));
  connect(inputTableModel_, SIGNAL(distributionChanged(const QModelIndex&)), this, SLOT(updateDistributionWidgets(const QModelIndex&)));
  connect(inputTableModel_, SIGNAL(correlationToChange()), this, SLOT(updateCorrelationTable()));
  connect(inputTableModel_, SIGNAL(distributionsChanged()), this, SLOT(updateCurrentVariableDistributionWidgets()));
  connect(inputTableModel_, SIGNAL(checked(bool)), inputTableHeaderView_, SLOT(setChecked(bool)));
  connect(inputTableModel_, SIGNAL(inferenceResultRequested(const QModelIndex&)), this, SLOT(openWizardToChooseInferenceResult(const QModelIndex&)));

  horizontalSplitter->addWidget(inputTableView_);

  // Distribution edition
  rightSideOfSplitterStackedWidget_ = new QStackedWidget;

  // If physical model is not valid: use a dummy widget
  rightSideOfSplitterStackedWidget_->addWidget(new QWidget);

  // If the selected variable is deterministic
  QScrollArea * scrollAreaForDeterministic = new QScrollArea;
  scrollAreaForDeterministic->setWidgetResizable(true);
  QGroupBox * groupBoxParameters = new QGroupBox(tr("Parameters"));
  QGridLayout * groupBoxParametersLayout = new QGridLayout(groupBoxParameters);
  QLabel * valueLabel = new QLabel(tr("Value"));
  groupBoxParametersLayout->addWidget(valueLabel, 0, 0);
  valueForDeterministicVariable_ = new ValueLineEdit;
  valueForDeterministicVariable_->setEnabled(false);
  groupBoxParametersLayout->addWidget(valueForDeterministicVariable_, 0, 1);
  groupBoxParametersLayout->setRowStretch(1, 1);
  scrollAreaForDeterministic->setWidget(groupBoxParameters);
  rightSideOfSplitterStackedWidget_->addWidget(scrollAreaForDeterministic);

  // If the selected variable is stochastic
  QScrollArea * rightScrollArea = new QScrollArea;
  rightScrollArea->setWidgetResizable(true);
  QFrame * rightFrame = new QFrame;
  QVBoxLayout * rightFrameLayout = new QVBoxLayout(rightFrame);

  //  PDF and CDF graphs
  QStackedWidget * plotStackedWidget = new QStackedWidget;

  QVector<PlotWidget*> listPlotWidgets;
  QWidget * widget = new QWidget;
  QVBoxLayout * vBox = new QVBoxLayout(widget);
  pdfPlot_ = new PlotWidget("distributionPDF");
  pdfPlot_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
  vBox->addWidget(pdfPlot_, 0, Qt::AlignHCenter|Qt::AlignTop);
  plotStackedWidget->addWidget(widget);
  listPlotWidgets.append(pdfPlot_);

  widget = new QWidget;
  vBox = new QVBoxLayout(widget);
  cdfPlot_ = new PlotWidget("distributionCDF");
  cdfPlot_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
  vBox->addWidget(cdfPlot_, 0, Qt::AlignHCenter|Qt::AlignTop);
  plotStackedWidget->addWidget(widget);
  listPlotWidgets.append(cdfPlot_);

  rightFrameLayout->addWidget(plotStackedWidget);

  pdf_cdfPlotsConfigurationWidget_ = new GraphConfigurationWidget(listPlotWidgets, QStringList(), QStringList(), GraphConfigurationWidget::PDF);
  connect(pdf_cdfPlotsConfigurationWidget_, SIGNAL(currentPlotChanged(int)), plotStackedWidget, SLOT(setCurrentIndex(int)));
  connect(rootTab, SIGNAL(currentChanged(int)), this, SLOT(showHideGraphConfigurationWidget(int)));

  //  parameters
  parameterLayout_ = new QVBoxLayout;
  parameterLayout_->addWidget(new QWidget);
  rightFrameLayout->addLayout(parameterLayout_);

  // truncation parameters
  truncationParamGroupBox_ = new CollapsibleGroupBox;
  truncationParamGroupBox_->setTitle(tr("Truncation parameters"));
  truncationParamGroupBox_->setExpanded(false);
  QGridLayout * truncationParamGroupBoxLayout = new QGridLayout(truncationParamGroupBox_);

  lowerBoundCheckBox_ = new QCheckBox(tr("Lower bound"));
  truncationParamGroupBoxLayout->addWidget(lowerBoundCheckBox_, 0, 0);
  upperBoundCheckBox_ = new QCheckBox(tr("Upper bound"));
  truncationParamGroupBoxLayout->addWidget(upperBoundCheckBox_, 1, 0);

  lowerBoundLineEdit_ = new ValueLineEdit;
  truncationParamGroupBoxLayout->addWidget(lowerBoundLineEdit_, 0, 1);
  upperBoundLineEdit_ = new ValueLineEdit;
  truncationParamGroupBoxLayout->addWidget(upperBoundLineEdit_, 1, 1);

  connect(lowerBoundCheckBox_, SIGNAL(stateChanged(int)), this, SLOT(truncationParametersStateChanged()));
  connect(upperBoundCheckBox_, SIGNAL(stateChanged(int)), this, SLOT(truncationParametersStateChanged()));
  connect(lowerBoundLineEdit_, SIGNAL(editingFinished()), this, SLOT(truncationParametersChanged()));
  connect(upperBoundLineEdit_, SIGNAL(editingFinished()), this, SLOT(truncationParametersChanged()));

  rightFrameLayout->addWidget(truncationParamGroupBox_);

  errorMessageLabel_ = new QLabel;
  errorMessageLabel_->setWordWrap(true);
  rightFrameLayout->addWidget(errorMessageLabel_);
  rightFrameLayout->addStretch();

  rightScrollArea->setWidget(rightFrame);
  rightSideOfSplitterStackedWidget_->addWidget(rightScrollArea);
  horizontalSplitter->addWidget(rightSideOfSplitterStackedWidget_);

  if (inputTableModel_->rowCount())
  {
    inputTableView_->selectRow(0);
    updateDistributionWidgets(inputTableView_->currentIndex());
  }
  else
    updateDistributionWidgets(inputTableModel_->index(-1, 0));

  tabLayout->addWidget(horizontalSplitter);
  rootTab->addTab(tab, tr("Marginals"));

  // Second Tab: correlation
  tab = new QWidget;
  tabLayout = new QVBoxLayout(tab);

  QGroupBox * groupBox = new QGroupBox(tr("Spearman's rank (Gaussian Copula)"));
  QVBoxLayout * groupBoxLayout = new QVBoxLayout(groupBox);
  correlationTableView_ = new CopyableTableView;
  SpinBoxDelegate * correlationDelegate = new SpinBoxDelegate;
  correlationTableView_->setItemDelegate(correlationDelegate);
  correlationTableModel_ = new CorrelationTableModel(physicalModel_, correlationTableView_);
  correlationTableView_->setModel(correlationTableModel_);
  correlationTableView_->setEditTriggers(QAbstractItemView::AllEditTriggers);
  groupBoxLayout->addWidget(correlationTableView_);
  tabLayout->addWidget(groupBox);

  correlationErrorMessage_ = new QLabel;
  correlationErrorMessage_->setWordWrap(true);
  tabLayout->addWidget(correlationErrorMessage_);
  connect(correlationTableModel_, SIGNAL(errorMessageChanged(QString)), this, SLOT(setCorrelationTabErrorMessage(QString)));

  rootTab->addTab(tab, tr("Correlation"));

  currentIndexTab_ = rootTab->currentIndex();
  setWidget(rootTab);
}


void ProbabilisticModelWindow::updateProbabilisticModel()
{
  // update stochastic inputs table
  updateStochasticInputsTable();

  // update plots and truncation widgets
  if (inputTableModel_->rowCount())
  {
    inputTableView_->selectRow(0);
    updateDistributionWidgets(inputTableView_->currentIndex());
  }
  else
    updateDistributionWidgets(inputTableModel_->index(-1, 0));

  // update correlation table
  updateCorrelationTable();
}


void ProbabilisticModelWindow::updateStochasticInputsTable()
{
  delete inputTableModel_;
  inputTableModel_ = new InputTableProbabilisticModel(physicalModel_, inputTableView_);

  inputTableView_->setModel(inputTableModel_);
  for (int i=0; i<inputTableModel_->rowCount(); ++i)
    inputTableView_->openPersistentEditor(inputTableModel_->index(i, 1));

  const bool headerViewIsChecked = (physicalModel_.hasStochasticInputs() && (physicalModel_.getComposedDistribution().getDimension() == physicalModel_.getInputs().getSize()));
  inputTableHeaderView_->setChecked(headerViewIsChecked);
    
  connect(inputTableModel_, SIGNAL(distributionChanged(const QModelIndex&)), this, SLOT(updateDistributionWidgets(const QModelIndex&)));
  connect(inputTableModel_, SIGNAL(correlationToChange()), this, SLOT(updateCorrelationTable()));
  connect(inputTableModel_, SIGNAL(distributionsChanged()), this, SLOT(updateCurrentVariableDistributionWidgets()));
  connect(inputTableModel_, SIGNAL(checked(bool)), inputTableHeaderView_, SLOT(setChecked(bool)));
  connect(inputTableModel_, SIGNAL(inferenceResultRequested(const QModelIndex&)), this, SLOT(openWizardToChooseInferenceResult(const QModelIndex&)));
}


void ProbabilisticModelWindow::updateCorrelationTable()
{
  delete correlationTableModel_;
  correlationTableModel_ = new CorrelationTableModel(physicalModel_, correlationTableView_);
  correlationTableView_->setModel(correlationTableModel_);
  connect(correlationTableModel_, SIGNAL(errorMessageChanged(QString)), this, SLOT(setCorrelationTabErrorMessage(QString)));
}


void ProbabilisticModelWindow::setCorrelationTabErrorMessage(const QString & message)
{
  correlationErrorMessage_->setText(QString("%1%2%3").arg("<font color=red>").arg(message).arg("</font>"));
  QTimeLine * time = new QTimeLine(7000, this);
  qtimelineList_.push_back(time);
  connect(time, SIGNAL(stateChanged(QTimeLine::State)), this, SLOT(reInitCorrelationErrorMessage(QTimeLine::State)));
  time->start();
}


void ProbabilisticModelWindow::reInitCorrelationErrorMessage(QTimeLine::State state)
{
  if (state == QTimeLine::NotRunning)
  {
    if (qtimelineList_.isEmpty())
      return;
    // remove the the first item of the list
    qtimelineList_.removeFirst();
    // if another QTimeLine started before the end of the previous one: do nothing
    if (qtimelineList_.size())
      return;
    // else initialize error message
    if (correlationErrorMessage_)
      correlationErrorMessage_->setText("");
  }
}


void ProbabilisticModelWindow::updateDistributionWidgets(const QModelIndex & index)
{
  if (!index.isValid())
  {
    rightSideOfSplitterStackedWidget_->setCurrentIndex(0);
    return;
  }

  if (index.row() != inputTableView_->currentIndex().row())
    inputTableView_->selectRow(index.row());

  Input input = physicalModel_.getInputs()[index.row()];
  String distributionName = input.getDistribution().getImplementation()->getClassName();

  // If the selected variable is deterministic
  if (distributionName == "Dirac")
  {
    rightSideOfSplitterStackedWidget_->setCurrentIndex(1);
    showHideGraphConfigurationWidget(-1);
    valueForDeterministicVariable_->setValue(input.getValue());
  }
  // If the selected variable is stochastic
  else
  {
    rightSideOfSplitterStackedWidget_->setCurrentIndex(2);
    truncationParamGroupBox_->setExpanded(false);

    // fill truncation parameters widgets
    updateTruncationParametersWidgets(index);

    // update parameters widgets
    updateDistributionParametersWidgets(index);

    // update plots
    updatePlots();
    showHideGraphConfigurationWidget(0);
  }
}


void ProbabilisticModelWindow::updateCurrentVariableDistributionWidgets()
{
  updateDistributionWidgets(inputTableView_->currentIndex());
}


void ProbabilisticModelWindow::updateDistributionParametersWidgets(const QModelIndex& index)
{
  if (!index.isValid())
    return;

  // update parameters widgets
  if (paramEditor_)
  {
    parameterLayout_->removeWidget(paramEditor_);
    paramEditor_->disconnect();
    paramEditor_->deleteLater();
    paramEditor_ = 0;
  }

  Input input = physicalModel_.getInputs()[index.row()];
  Distribution inputDistribution = input.getDistribution();
  String distributionName = inputDistribution.getImplementation()->getClassName();
  Distribution::NumericalPointWithDescriptionCollection parameters = DistributionDictionary::GetParametersCollection(inputDistribution);
  UnsignedInteger parametersType = input.getDistributionParametersType();
  UnsignedInteger nbParameters = parameters[parametersType].getSize();

  if (distributionName == "TruncatedDistribution")
  {
    TruncatedDistribution dist(*dynamic_cast<TruncatedDistribution*>(inputDistribution.getImplementation().get()));
    parameters = DistributionDictionary::GetParametersCollection(dist.getDistribution().getImplementation());
    nbParameters = parameters[parametersType].getSize();
  }
  else if (distributionName == "TruncatedNormal")
  {
    nbParameters = 2;
  }

  // check nbParameters value
  if (!nbParameters)
    return;

  if (nbParameters >= 5)
    throw InternalException(HERE) << "Error: Number of parameters invalid :" << nbParameters;

  // fill parameters widgets
  paramEditor_ = new QGroupBox(tr("Parameters"));
  QGridLayout * lay = new QGridLayout(paramEditor_);

  // combobox to choose the type of distribution configuration
  QLabel * label = new QLabel(tr("Type"));
  lay->addWidget(label, 0, 0);
  QComboBox * selectParametersTypeCombo = new QComboBox;
  for (UnsignedInteger i=0; i<parameters.getSize(); ++i)
  {
    QStringList parametersNamesList;
    for (UnsignedInteger j=0; j<parameters[i].getDescription().getSize(); ++j)
      parametersNamesList << QString::fromUtf8(parameters[i].getDescription()[j].c_str());

    selectParametersTypeCombo->addItem(parametersNamesList.join(", "));
  }
  selectParametersTypeCombo->setCurrentIndex(parametersType);
  connect(selectParametersTypeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(typeDistributionParametersChanged(int)));
  lay->addWidget(selectParametersTypeCombo, 0, 1);

  // lineEdits to set the distribution parameters
  Description parametersName = parameters[parametersType].getDescription();
  for (UnsignedInteger i=0; i<nbParameters; ++i)
  {
    parameterValuesLabel_[i] = new QLabel(parametersName[i].c_str());
    parameterValuesEdit_[i] = new ValueLineEdit(parameters[parametersType][i]);
    connect(parameterValuesEdit_[i], SIGNAL(editingFinished()), this, SLOT(distributionParametersChanged()));
    parameterValuesLabel_[i]->setBuddy(parameterValuesEdit_[i]);
    lay->addWidget(parameterValuesLabel_[i], i+1, 0);
    lay->addWidget(parameterValuesEdit_[i], i+1, 1);
  }
  parameterLayout_->insertWidget(0, paramEditor_);
}


void ProbabilisticModelWindow::updateTruncationParametersWidgets(const QModelIndex & index)
{
  if (!index.isValid())
    return;

  Input input = physicalModel_.getInputs()[index.row()];
  Distribution inputDistribution = input.getDistribution();
  String distributionName = inputDistribution.getImplementation()->getClassName();

  SignalBlocker lowerBoundCheckBoxBlocker(lowerBoundCheckBox_);
  SignalBlocker upperBoundCheckBoxBlocker(upperBoundCheckBox_);
  SignalBlocker lowerBoundLineEditBlocker(lowerBoundLineEdit_);
  SignalBlocker upperBoundLineEditBlocker(upperBoundLineEdit_);

  // clear truncation parameters
  lowerBoundLineEdit_->deactivate();
  upperBoundLineEdit_->deactivate();
  lowerBoundCheckBox_->setChecked(false);
  upperBoundCheckBox_->setChecked(false);

  // fill truncation parameters widgets
  if (distributionName == "TruncatedDistribution")
  {
    TruncatedDistribution dist(*dynamic_cast<TruncatedDistribution*>(inputDistribution.getImplementation().get()));

    lowerBoundCheckBox_->setChecked(dist.getFiniteLowerBound());
    upperBoundCheckBox_->setChecked(dist.getFiniteUpperBound());
    if (dist.getFiniteLowerBound())
      lowerBoundLineEdit_->setValue(dist.getLowerBound());

    if (dist.getFiniteUpperBound())
      upperBoundLineEdit_->setValue(dist.getUpperBound());

    truncationParamGroupBox_->setExpanded(true);
  }
  else if (distributionName == "TruncatedNormal")
  {
    TruncatedNormal dist(*dynamic_cast<TruncatedNormal*>(inputDistribution.getImplementation().get()));

    lowerBoundCheckBox_->setChecked(true);
    upperBoundCheckBox_->setChecked(true);
    lowerBoundLineEdit_->setValue(dist.getA());
    upperBoundLineEdit_->setValue(dist.getB());
    truncationParamGroupBox_->setExpanded(true);
  }
}


void ProbabilisticModelWindow::showHideGraphConfigurationWidget(int indexTab)
{
  if (indexTab != -1)
    currentIndexTab_ = indexTab;

  switch (indexTab)
  {
    case 0: // distribution graph
    {
      if (pdf_cdfPlotsConfigurationWidget_)
      {
        if (rightSideOfSplitterStackedWidget_->currentIndex() == 2
            && (windowState() == Qt::WindowFullScreen || windowState() == (Qt::WindowActive|Qt::WindowMaximized)))
          emit graphWindowActivated(pdf_cdfPlotsConfigurationWidget_);
        else
          emit graphWindowDeactivated();
      }
      break;
    }
    default:
      emit graphWindowDeactivated();
      break;
  }
}


void ProbabilisticModelWindow::showHideGraphConfigurationWidget(Qt::WindowStates oldState, Qt::WindowStates newState)
{
  if (oldState == Qt::WindowMaximized)
    return;

  if (newState == Qt::WindowFullScreen || newState == (Qt::WindowActive|Qt::WindowMaximized))
    showHideGraphConfigurationWidget(currentIndexTab_);
  else if (newState == Qt::WindowNoState || newState == Qt::WindowMinimized || newState == (Qt::WindowActive|Qt::WindowMinimized))
    showHideGraphConfigurationWidget(-1);
}


void ProbabilisticModelWindow::updatePlots()
{
  QModelIndex index = inputTableView_->currentIndex();
  Input input = physicalModel_.getInputs()[index.row()];
  pdfPlot_->clear();
  cdfPlot_->clear();
  pdfPlot_->plotPDFCurve(input.getDistribution());
  pdfPlot_->setAxisTitle(QwtPlot::xBottom, QString::fromUtf8(input.getName().c_str()));
  cdfPlot_->plotCDFCurve(input.getDistribution());
  cdfPlot_->setAxisTitle(QwtPlot::xBottom, QString::fromUtf8(input.getName().c_str()));
}


void ProbabilisticModelWindow::distributionParametersChanged()
{
  QModelIndex index = inputTableView_->currentIndex();
  Input input = physicalModel_.getInputs()[inputTableView_->currentIndex().row()];
  Distribution inputDistribution = input.getDistribution();
  String distributionName = inputDistribution.getImplementation()->getClassName();
  UnsignedInteger parametersType = input.getDistributionParametersType();

  if (distributionName == "TruncatedDistribution")
  {
    TruncatedDistribution truncatedDistribution(*dynamic_cast<TruncatedDistribution*>(inputDistribution.getImplementation().get()));
    Distribution distribution = truncatedDistribution.getDistribution();
    NumericalPointWithDescription parameters = DistributionDictionary::GetParametersCollection(distribution)[parametersType];
    try
    {
      for (UnsignedInteger i=0; i<parameters.getSize(); ++i)
        parameters[i] = parameterValuesEdit_[i]->value();

      if (parameters == DistributionDictionary::GetParametersCollection(distribution)[parametersType])
        return;

      DistributionDictionary::UpdateDistribution(distribution, parameters, parametersType);

      // create a new TruncatedDistribution (that fixes a bug...)
      TruncatedDistribution newTruncatedDistribution;
      newTruncatedDistribution.setDistribution(distribution);
      newTruncatedDistribution.setFiniteLowerBound(false);
      newTruncatedDistribution.setFiniteUpperBound(false);

      if (truncatedDistribution.getFiniteLowerBound())
        newTruncatedDistribution.setLowerBound(truncatedDistribution.getLowerBound()); // FiniteLowerBound = true

      if (truncatedDistribution.getFiniteUpperBound())
        newTruncatedDistribution.setUpperBound(truncatedDistribution.getUpperBound()); // FiniteUpperBound = true

      // update input distribution
      physicalModel_.blockNotification(true);
      physicalModel_.setDistribution(input.getName(), newTruncatedDistribution);
      physicalModel_.blockNotification(false);
      updatePlots();
    }
    catch(std::exception & ex)
    {
      physicalModel_.blockNotification(false);
      qDebug() << "ProbabilisticModelWindow::distributionParametersChanged invalid parameters:"
               << parameters.__str__().data() << " for distribution:" << distributionName.data();
      updateDistributionParametersWidgets(index);
      setTemporaryErrorMessage(ex.what());
    }
  }
  else
  {
    NumericalPointWithDescription parameters = DistributionDictionary::GetParametersCollection(inputDistribution)[parametersType];
    UnsignedInteger nbParameters = parameters.getSize();
    if (distributionName == "TruncatedNormal")
      nbParameters = 2; // the 2 truncation parameters are updated in truncationParametersChanged

    try
    {
      for (UnsignedInteger i=0; i<nbParameters; ++i)
        parameters[i] = parameterValuesEdit_[i]->value();

      if (parameters == DistributionDictionary::GetParametersCollection(inputDistribution)[parametersType])
        return;

      DistributionDictionary::UpdateDistribution(inputDistribution, parameters, parametersType);
      physicalModel_.blockNotification(true);
      physicalModel_.setDistribution(input.getName(), inputDistribution);
      physicalModel_.blockNotification(false);
      updatePlots();
    }
    catch(std::exception & ex)
    {
      physicalModel_.blockNotification(false);
      qDebug() << "ProbabilisticModelWindow::distributionParametersChanged invalid parameters:"
               << parameters.__str__().data() << " for distribution:" << distributionName.data();
      updateDistributionParametersWidgets(index);
      setTemporaryErrorMessage(ex.what());
    }
  }
}


void ProbabilisticModelWindow::typeDistributionParametersChanged(int comboIndex)
{
  QModelIndex index = inputTableView_->currentIndex();
  Input input = physicalModel_.getInputs()[index.row()];
  physicalModel_.setDistributionParametersType(input.getName(), comboIndex);
  updateDistributionParametersWidgets(index);
}


void ProbabilisticModelWindow::truncationParametersChanged()
{
  QModelIndex index = inputTableView_->currentIndex();
  Input input = physicalModel_.getInputs()[index.row()];
  Distribution inputDistribution = input.getDistribution();

  try
  {
    if (inputDistribution.getImplementation()->getClassName() == "TruncatedNormal")
    {
      TruncatedNormal dist(*dynamic_cast<TruncatedNormal*>(inputDistribution.getImplementation().get()));

      if (lowerBoundCheckBox_->isChecked())
        dist.setA(lowerBoundLineEdit_->value());
      if (upperBoundCheckBox_->isChecked())
        dist.setB(upperBoundLineEdit_->value());

      physicalModel_.blockNotification(true);
      physicalModel_.setDistribution(input.getName(), dist);
      physicalModel_.blockNotification(false);
    }
    else
    {
      TruncatedDistribution truncatedDistribution(*dynamic_cast<TruncatedDistribution*>(inputDistribution.getImplementation().get()));
      if (lowerBoundCheckBox_->isChecked())
        truncatedDistribution.setLowerBound(lowerBoundLineEdit_->value());
      if (upperBoundCheckBox_->isChecked())
        truncatedDistribution.setUpperBound(upperBoundLineEdit_->value());

      physicalModel_.blockNotification(true);
      physicalModel_.setDistribution(input.getName(), truncatedDistribution);
      physicalModel_.blockNotification(false);
    }

    updatePlots();
  }
  catch (std::exception & ex)
  {
    physicalModel_.blockNotification(false);
    qDebug() << "Error: ProbabilisticModelWindow::truncationParametersChanged\n";
    updateTruncationParametersWidgets(index);
    setTemporaryErrorMessage(ex.what());
  }
}


void ProbabilisticModelWindow::truncationParametersStateChanged()
{
  QModelIndex index = inputTableView_->currentIndex();
  Input input = Input(physicalModel_.getInputs()[index.row()]);
  Distribution inputDistribution = input.getDistribution();
  String distributionName = inputDistribution.getImplementation()->getClassName();

  try
  {
    // If the two checkboxes are unchecked
    // <=> if the distribution was truncated before a checkbox state has changed
    // <=> if now the distribution is not truncated
    if (!lowerBoundCheckBox_->isChecked() && !upperBoundCheckBox_->isChecked())
    {
      lowerBoundLineEdit_->deactivate();
      upperBoundLineEdit_->deactivate();
      // find the not truncated distribution
      if (distributionName == "TruncatedDistribution")
      {
        TruncatedDistribution dist(*dynamic_cast<TruncatedDistribution*>(inputDistribution.getImplementation().get()));
        inputDistribution = dist.getDistribution();
      }
      else if (distributionName == "TruncatedNormal")
      {
        TruncatedNormal dist(*dynamic_cast<TruncatedNormal*>(inputDistribution.getImplementation().get()));
        inputDistribution = Normal(dist.getMu(), dist.getSigma());
      }
      else
        throw;

      physicalModel_.blockNotification(true);
      physicalModel_.setDistribution(input.getName(), inputDistribution);
      physicalModel_.blockNotification(false);

      // update plots
      updatePlots();
    }
    // if at least one checkbox is checked
    // <=> if the distribution is truncated
    else
    {
      // update widgets
      Interval truncatureInterval(NumericalPoint(1), NumericalPoint(1), Interval::BoolCollection(1, false), Interval::BoolCollection(1, false));
      Distribution distribution;
      TruncatedDistribution truncatedDistribution;
      if (distributionName == "TruncatedDistribution")
      {
        truncatedDistribution = TruncatedDistribution(*dynamic_cast<TruncatedDistribution*>(inputDistribution.getImplementation().get()));
        distribution = truncatedDistribution.getDistribution();
        truncatureInterval = Interval(truncatedDistribution.getLowerBound(), truncatedDistribution.getUpperBound());
        truncatureInterval.setFiniteLowerBound(Interval::BoolCollection(1, truncatedDistribution.getFiniteLowerBound()));
        truncatureInterval.setFiniteUpperBound(Interval::BoolCollection(1, truncatedDistribution.getFiniteUpperBound()));
      }
      else if (distributionName == "TruncatedNormal")
      {
        TruncatedNormal dist(*dynamic_cast<TruncatedNormal*>(inputDistribution.getImplementation().get()));
        distribution = Normal(dist.getMu(), dist.getSigma());
        truncatureInterval = Interval(dist.getA(), dist.getB());
      }
      else
      {
        distribution = inputDistribution;
        truncatureInterval.setLowerBound(distribution.getRange().getLowerBound());
        truncatureInterval.setUpperBound(distribution.getRange().getUpperBound());
      }

      QCheckBox * checkbox = qobject_cast<QCheckBox*>(sender());
      if (checkbox == lowerBoundCheckBox_)
      {
        if (lowerBoundCheckBox_->isChecked())
        {
          double distMean = distribution.getMean()[0];
          double distStd = distribution.getStandardDeviation()[0];
          double lowerBound = distMean - distStd;
          if (truncatureInterval.getFiniteUpperBound()[0])
            if (lowerBound >= truncatureInterval.getUpperBound()[0])
              lowerBound = truncatureInterval.getUpperBound()[0] - 0.1 * distStd;
          truncatureInterval.setLowerBound(NumericalPoint(1, lowerBound));
          SignalBlocker blocker(lowerBoundLineEdit_);
          lowerBoundLineEdit_->setValue(lowerBound);
          truncatureInterval.setFiniteLowerBound(Interval::BoolCollection(1, true));
        }
        else
        {
          lowerBoundLineEdit_->deactivate();
          truncatureInterval.setFiniteLowerBound(Interval::BoolCollection(1, false));
        }
      }
      else if (checkbox == upperBoundCheckBox_)
      {
        if (upperBoundCheckBox_->isChecked())
        {
          double distMean = distribution.getMean()[0];
          double distStd = distribution.getStandardDeviation()[0];
          double upperBound = distMean + distStd;
          if (truncatureInterval.getFiniteLowerBound()[0])
            if (upperBound <= truncatureInterval.getLowerBound()[0])
              upperBound = truncatureInterval.getLowerBound()[0] + 0.1 * distStd;
          truncatureInterval.setUpperBound(NumericalPoint(1, upperBound));
          SignalBlocker blocker(upperBoundLineEdit_);
          upperBoundLineEdit_->setValue(upperBound);
          truncatureInterval.setFiniteUpperBound(Interval::BoolCollection(1, true));
        }
        else
        {
          upperBoundLineEdit_->deactivate();
          truncatureInterval.setFiniteUpperBound(Interval::BoolCollection(1, false));
        }
      }

      // update Distribution
      //TODO after fixing problem with CTR of TruncatedDistribution(Distribution, Interval) in OT do:
      // truncatedDistribution = TruncatedDistribution(distribution, truncatureInterval)
      if (!(truncatureInterval.getFiniteLowerBound()[0] && truncatureInterval.getFiniteUpperBound()[0]))
      {
        if (truncatureInterval.getFiniteLowerBound()[0])
          truncatedDistribution = TruncatedDistribution(distribution, truncatureInterval.getLowerBound()[0]);
        else
          truncatedDistribution = TruncatedDistribution(distribution, truncatureInterval.getUpperBound()[0], TruncatedDistribution::UPPER);
      }
      else
        truncatedDistribution = TruncatedDistribution(distribution, truncatureInterval.getLowerBound()[0], truncatureInterval.getUpperBound()[0]);

      physicalModel_.blockNotification(true);
      physicalModel_.setDistribution(input.getName(), truncatedDistribution);
      physicalModel_.blockNotification(false);

      // update plots
      updatePlots();
    }
  }
  catch (std::exception & ex)
  {
    physicalModel_.blockNotification(false);
    qDebug() << "Error: ProbabilisticModelWindow::truncationParametersStateChanged\n";
    updateTruncationParametersWidgets(index);
    setTemporaryErrorMessage(ex.what());
  }
}


void ProbabilisticModelWindow::openWizardToChooseInferenceResult(const QModelIndex& inputIndex)
{
  bool otStudyHasInferenceResults = false;
  // we need at least one inference analysis result to open the wizard
  for (UnsignedInteger i=0; i<otStudy_.getAnalyses().getSize(); ++i)
  {
    if (otStudy_.getAnalyses()[i].getImplementation()->getClassName() == "InferenceAnalysis")
    {
      if (dynamic_cast<InferenceAnalysis*>(otStudy_.getAnalyses()[i].getImplementation().get())->analysisLaunched())
      {
        otStudyHasInferenceResults = true;
        break;
      }
    }
  }

  if (!otStudyHasInferenceResults)
  {
    QMessageBox::critical(this, tr("Error"), tr("The current study has not inference analyses results."));
    return;
  }
  else
  {
    InferenceResultWizard * wizard = new InferenceResultWizard(otStudy_, this);
    if (wizard->exec())
    {
      // update the input
      const Input input(physicalModel_.getInputs()[inputIndex.row()]);
      physicalModel_.blockNotification(true);
      physicalModel_.setDistribution(input.getName(), wizard->getDistribution());
      physicalModel_.setDistributionParametersType(input.getName(), 0);
      physicalModel_.blockNotification(false);
      updateDistributionWidgets(inputIndex);
    }
  }
}
}
