// ProbabilisticModelWindow.cxx

#include "otgui/ProbabilisticModelWindow.hxx"

#include "otgui/ComboBoxDelegate.hxx"
#include "otgui/SpinBoxDelegate.hxx"
#include "otgui/InputTableProbabilisticModel.hxx"
#include "otgui/CorrelationTableModel.hxx"

#include "Normal.hxx"
#include "TruncatedDistribution.hxx"
#include "TruncatedNormal.hxx"

#include <QSplitter>
#include <QHeaderView>
#include <QScrollArea>
#include <QPushButton>
#include <QStackedLayout>

using namespace OT;

namespace OTGUI {

ProbabilisticModelWindow::ProbabilisticModelWindow(ProbabilisticModelItem * item)
  : OTguiSubWindow(item)
  , physicalModel_(item->getPhysicalModel())
  , paramEditor_(0)
{
  connect(item, SIGNAL(inputChanged()), this, SLOT(updateProbabilisticModel()));
  connect(item, SIGNAL(physicalModelChanged(const PhysicalModel&)), this, SLOT(updatePhysicalModel(const PhysicalModel&)));
  buildInterface();
  connect(this, SIGNAL(windowStateChanged(Qt::WindowStates, Qt::WindowStates)), this, SLOT(showHideGraphConfigurationWidget(Qt::WindowStates, Qt::WindowStates)));
}


void ProbabilisticModelWindow::buildInterface()
{
  QTabWidget * rootTab = new QTabWidget;
  
  // First Tab: marginals
  QWidget * tab = new QWidget;
  QHBoxLayout * tabLayout = new QHBoxLayout(tab);

  QSplitter * horizontalSplitter = new QSplitter;

  // Inputs
  QWidget * leftSideOfSplitter = new QWidget;
  QVBoxLayout * leftSideOfSplitterLayout = new QVBoxLayout(leftSideOfSplitter);

  //   stochastic inputs table
  inputTableView_ = new QTableView;
  QStringList items = QStringList()<<tr("Beta")<<tr("Gamma")<<tr("Gumbel")<<tr("LogNormal")<<tr("Normal")<<tr("Uniform");
  ComboBoxDelegate * delegate = new ComboBoxDelegate(items);
  inputTableView_->setItemDelegateForColumn(1, delegate);
  inputTableView_->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
  inputTableView_->setSelectionBehavior(QAbstractItemView::SelectRows);

  inputTableModel_ = new InputTableProbabilisticModel(physicalModel_);
  inputTableView_->setModel(inputTableModel_);

  for (int i = 0; i < inputTableModel_->rowCount(); ++i)
    inputTableView_->openPersistentEditor(inputTableModel_->index(i, 1));

  connect(inputTableView_, SIGNAL(clicked(QModelIndex)), this, SLOT(updateDistributionWidgets(const QModelIndex&)));
  connect(inputTableModel_, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(updateDistributionWidgets(const QModelIndex&)));

  leftSideOfSplitterLayout->addWidget(inputTableView_);

  //   deterministic inputs list
  deterministicInputsComboBox_ = new QComboBox;
  populateDeterministicInputsComboBox();
  connect(deterministicInputsComboBox_, SIGNAL(activated(int)), this, SLOT(addInputRequested(int)));

  QPushButton * removeButton = new QPushButton(QIcon(":/images/list-remove.png"), "Remove");
  connect(removeButton, SIGNAL(clicked(bool)), this, SLOT(removeInputRequested()));

  QHBoxLayout * buttonsLayout = new QHBoxLayout;
  buttonsLayout->addWidget(deterministicInputsComboBox_);
  buttonsLayout->addWidget(removeButton);
  leftSideOfSplitterLayout->addLayout(buttonsLayout);

  horizontalSplitter->addWidget(leftSideOfSplitter);

  // Distribution edition
  QScrollArea * rightScrollArea = new QScrollArea;
  rightScrollArea->setWidgetResizable(true);
  rightFrame_ = new QFrame;
  QVBoxLayout * rightLayout = new QVBoxLayout(rightFrame_);

  //  PDF and CDF graphs
  QStackedLayout * plotLayout = new QStackedLayout;

  QVector<PlotWidget*> listPlotWidgets;
  QWidget * widget = new QWidget;
  QVBoxLayout * vBox = new QVBoxLayout(widget);
  pdfPlot_ = new PlotWidget;
  pdfPlot_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  vBox->addWidget(pdfPlot_, 0, Qt::AlignHCenter|Qt::AlignTop);
  plotLayout->addWidget(widget);
  listPlotWidgets.append(pdfPlot_);

  widget = new QWidget;
  vBox = new QVBoxLayout(widget);
  cdfPlot_ = new PlotWidget;
  cdfPlot_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  vBox->addWidget(cdfPlot_, 0, Qt::AlignHCenter|Qt::AlignTop);
  plotLayout->addWidget(widget);
  listPlotWidgets.append(cdfPlot_);

  pdf_cdfPlotsConfigurationWidget_ = new GraphConfigurationWidget(listPlotWidgets, QStringList(), QStringList(), GraphConfigurationWidget::PDF);
  connect(pdf_cdfPlotsConfigurationWidget_, SIGNAL(currentPlotChanged(int)), plotLayout, SLOT(setCurrentIndex(int)));

  rightLayout->addLayout(plotLayout);

  //  parameters
  parameterLayout_ = new QVBoxLayout;
  parameterLayout_->addWidget(new QWidget);
  rightLayout->addLayout(parameterLayout_);

  // advanced group: truncation parameters
  advancedGroup_ = new QGroupBox(tr("Truncation parameters"));
  QVBoxLayout * advancedGroupLayout = new QVBoxLayout(advancedGroup_);
  advancedGroup_->setCheckable(true);
  advancedGroup_->setStyleSheet("QGroupBox::indicator::unchecked {image: url(:/images/down_arrow.png);}\
                                QGroupBox::indicator::checked {image: url(:/images/up_arrow.png);}");

  connect(advancedGroup_, SIGNAL(toggled(bool)), this, SLOT(showHideAdvancedWidgets(bool)));

  advancedWidgets_ = new QWidget;
  QGridLayout * advancedWidgetsLayout = new QGridLayout(advancedWidgets_);

  lowerBoundCheckBox_ = new QCheckBox(tr("Lower bound"));
  advancedWidgetsLayout->addWidget(lowerBoundCheckBox_, 0, 0);
  upperBoundCheckBox_ = new QCheckBox(tr("Upper bound"));
  advancedWidgetsLayout->addWidget(upperBoundCheckBox_, 1, 0);

  lowerBoundLineEdit_ = new QLineEdit;
  advancedWidgetsLayout->addWidget(lowerBoundLineEdit_, 0, 1);
  upperBoundLineEdit_ = new QLineEdit;
  advancedWidgetsLayout->addWidget(upperBoundLineEdit_, 1, 1);

  connect(lowerBoundCheckBox_, SIGNAL(stateChanged(int)), this, SLOT(truncationParametersStateChanged()));
  connect(upperBoundCheckBox_, SIGNAL(stateChanged(int)), this, SLOT(truncationParametersStateChanged()));
  connect(lowerBoundLineEdit_, SIGNAL(editingFinished()), this, SLOT(truncationParametersChanged()));
  connect(upperBoundLineEdit_, SIGNAL(editingFinished()), this, SLOT(truncationParametersChanged()));

  advancedGroupLayout->addWidget(advancedWidgets_);
  advancedGroupLayout->addStretch();
  rightLayout->addWidget(advancedGroup_);
  rightScrollArea->setWidget(rightFrame_);
  horizontalSplitter->addWidget(rightScrollArea);

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
  tabLayout = new QHBoxLayout(tab);

  QGroupBox * groupBox = new QGroupBox(tr("Spearman's rank"));
  QVBoxLayout * groupBoxLayout = new QVBoxLayout(groupBox);
  correlationTableView_ = new QTableView;
  SpinBoxDelegate * correlationDelegate = new SpinBoxDelegate;
  correlationTableView_->setItemDelegate(correlationDelegate);
  correlationTableModel_ = new CorrelationTableModel(physicalModel_);
  correlationTableView_->setModel(correlationTableModel_);
  correlationTableView_->setEditTriggers(QAbstractItemView::AllEditTriggers);
  groupBoxLayout->addWidget(correlationTableView_);
  tabLayout->addWidget(groupBox);

  rootTab->addTab(tab, tr("Correlation"));

  setWidget(rootTab);
}

void ProbabilisticModelWindow::showHideAdvancedWidgets(bool show)
{
  if (show)
    advancedWidgets_->show();
  else
    advancedWidgets_->hide();
}


void ProbabilisticModelWindow::addInputRequested(int comboIndex)
{
  if (comboIndex)
  {
    QString inputName = deterministicInputsComboBox_->currentText();
    physicalModel_.setInputDistribution(inputName.toStdString(), Normal(0, 1));
  }
}


void ProbabilisticModelWindow::removeInputRequested()
{
  if (inputTableView_->selectionModel()->hasSelection())
  {
    std::string inputName =  inputTableModel_->data(inputTableView_->currentIndex(), Qt::DisplayRole).toString().toStdString();
    physicalModel_.setInputDistribution(inputName, Dirac(physicalModel_.getInputByName(inputName).getValue()));
  }
}


void ProbabilisticModelWindow::updateDeterministicInputsComboBox()
{
  deterministicInputsComboBox_->clear();
  delete deterministicInputsComboBoxModel_;
  populateDeterministicInputsComboBox();
}


void ProbabilisticModelWindow::populateDeterministicInputsComboBox()
{
  deterministicInputsComboBoxModel_ = new QStandardItemModel(1, 1);
  QStandardItem * firstItem = new QStandardItem(QIcon(":/images/list-add.png"), "Add");
  firstItem->setBackground(QBrush(QColor(200, 200, 200)));
  deterministicInputsComboBoxModel_->setItem(0, 0, firstItem);
  deterministicInputsComboBox_->setModel(deterministicInputsComboBoxModel_);

  for (int i=0; i<physicalModel_.getInputs().getSize(); ++i)
    if (!physicalModel_.getInputs()[i].isStochastic())
      deterministicInputsComboBox_->addItem(physicalModel_.getInputs()[i].getName().c_str());
  deterministicInputsComboBox_->setCurrentIndex(0);
}


void ProbabilisticModelWindow::updatePhysicalModel(const PhysicalModel & physicalModel)
{
  physicalModel_ = physicalModel;
  updateProbabilisticModel();
}


void ProbabilisticModelWindow::updateProbabilisticModel()
{
  // update stochastic inputs table
  updateStochasticInputsTable();

  // update plots and truncation widgets
  updateDistributionWidgets(inputTableView_->currentIndex());

  // update deterministic inputs list
  updateDeterministicInputsComboBox();

  // update correlation table
  updateCorrelationTable();
}


void ProbabilisticModelWindow::updateStochasticInputsTable()
{
  delete inputTableModel_;
  inputTableModel_ = new InputTableProbabilisticModel(physicalModel_);
  inputTableView_->setModel(inputTableModel_);
  for (int i = 0; i < inputTableModel_->rowCount(); ++i)
    inputTableView_->openPersistentEditor(inputTableModel_->index(i, 1));
  connect(inputTableModel_, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(updateDistributionWidgets(const QModelIndex&)));
}


void ProbabilisticModelWindow::updateCorrelationTable()
{
  delete correlationTableModel_;
  correlationTableModel_ = new CorrelationTableModel(physicalModel_);
  correlationTableView_->setModel(correlationTableModel_);
}


void ProbabilisticModelWindow::updateDistributionWidgets(const QModelIndex & index)
{
  // update parameters widgets
  if (paramEditor_)
  {
    parameterLayout_->removeWidget(paramEditor_);
    paramEditor_->disconnect();
    paramEditor_->deleteLater();
    paramEditor_ = 0;
  }

  if (!index.isValid())
  {
    if (!inputTableView_->model()->rowCount())
    {
      rightFrame_->hide();
      return;
    }
    else
      inputTableView_->selectRow(0);
  }
  rightFrame_->show();
  advancedGroup_->setChecked(false);

  if (index.row() != inputTableView_->currentIndex().row() || index.column() == 1)
    inputTableView_->selectRow(index.row());

  QString inputName =  inputTableModel_->data(inputTableView_->currentIndex(), Qt::DisplayRole).toString();
  Distribution inputDistribution = physicalModel_.getInputByName(inputName.toStdString()).getDistribution();
  std::string distributionName = inputDistribution.getImplementation()->getClassName();

  const NumericalPointWithDescription parameters = inputDistribution.getParametersCollection()[0];
  int nbParameters = parameters.getSize();

  lowerBoundCheckBox_->blockSignals(true);
  upperBoundCheckBox_->blockSignals(true);
  lowerBoundLineEdit_->blockSignals(true);
  upperBoundLineEdit_->blockSignals(true);

  // clear truncation parameters
  lowerBoundLineEdit_->clear();
  lowerBoundLineEdit_->setEnabled(false);
  upperBoundLineEdit_->clear();
  upperBoundLineEdit_->setEnabled(false);
  lowerBoundCheckBox_->setChecked(false);
  upperBoundCheckBox_->setChecked(false);

  // fill truncation parameters widgets
  if (distributionName == "TruncatedDistribution")
  {
    TruncatedDistribution * dist = dynamic_cast<TruncatedDistribution*>(&*inputDistribution.getImplementation());

    lowerBoundCheckBox_->setChecked(dist->getFiniteLowerBound());
    upperBoundCheckBox_->setChecked(dist->getFiniteUpperBound());
    if (dist->getFiniteLowerBound())
    {
      lowerBoundLineEdit_->setText(QString::number(dist->getLowerBound()));
      lowerBoundLineEdit_->setEnabled(true);
    }
    if (dist->getFiniteUpperBound())
    {
      upperBoundLineEdit_->setText(QString::number(dist->getUpperBound()));
      upperBoundLineEdit_->setEnabled(true);
    }
    advancedGroup_->setChecked(true);

    nbParameters = dist->getDistribution().getParametersCollection()[0].getSize();
  }
  else if (distributionName == "TruncatedNormal")
  {
    TruncatedNormal * dist = dynamic_cast<TruncatedNormal*>(&*inputDistribution.getImplementation());

    lowerBoundCheckBox_->setChecked(true);
    upperBoundCheckBox_->setChecked(true);
    lowerBoundLineEdit_->setText(QString::number(dist->getA()));
    lowerBoundLineEdit_->setEnabled(true);
    upperBoundLineEdit_->setText(QString::number(dist->getB()));
    upperBoundLineEdit_->setEnabled(true);
    advancedGroup_->setChecked(true);

    nbParameters = 2;
  }
  lowerBoundCheckBox_->blockSignals(false);
  upperBoundCheckBox_->blockSignals(false);
  lowerBoundLineEdit_->blockSignals(false);
  upperBoundLineEdit_->blockSignals(false);

  // fill parameters widgets
  if (nbParameters)
  {
    if (nbParameters >= 5)
      std::cout << "Number of parameters invalid :"<<nbParameters<<std::endl;
  
    Description parametersName = parameters.getDescription();

    paramEditor_ = new QGroupBox(tr("Parameters"));
    QGridLayout * lay = new QGridLayout(paramEditor_);
    for (int i=0; i<nbParameters; ++i)
    {
      parameterValuesLabel_[i] = new QLabel(parametersName[i].c_str());
      parameterValuesEdit_[i] = new QLineEdit(QString::number(parameters[i]));
      connect(parameterValuesEdit_[i], SIGNAL(editingFinished()), this, SLOT(updateDistribution()));
      parameterValuesLabel_[i]->setBuddy(parameterValuesEdit_[i]);
      lay->addWidget(parameterValuesLabel_[i], i, 0);
      lay->addWidget(parameterValuesEdit_[i], i, 1);
    }
    parameterLayout_->insertWidget(0, paramEditor_);
  }

  // update plots
  updatePlots(inputDistribution);

  if (inputTableModel_->rowCount())
    emit graphWindowActivated(pdf_cdfPlotsConfigurationWidget_);
  else
    emit graphWindowDeactivated(pdf_cdfPlotsConfigurationWidget_);
}


void ProbabilisticModelWindow::showHideGraphConfigurationWidget(Qt::WindowStates oldState, Qt::WindowStates newState)
{
  if (newState == 4 || newState == 8 || newState == 10)
  {
    if (inputTableModel_->rowCount())
      emit graphWindowActivated(pdf_cdfPlotsConfigurationWidget_);
  }
  else if (newState == 0 || newState == 1 || newState == 2 || newState == 9)
    emit graphWindowDeactivated(pdf_cdfPlotsConfigurationWidget_);
}


void ProbabilisticModelWindow::updatePlots(Distribution inputDistribution)
{
  pdfPlot_->clear();
  cdfPlot_->clear();
  pdfPlot_->plotPDFCurve(inputDistribution);
  cdfPlot_->plotCDFCurve(inputDistribution);
}


void ProbabilisticModelWindow::updateDistribution()
{
  QModelIndex index = inputTableView_->currentIndex();
  Input input = Input(physicalModel_.getInputs()[index.data(Qt::UserRole).toInt()]);
  Distribution inputDistribution = input.getDistribution();
  std::string distributionName = inputDistribution.getImplementation()->getClassName();

  if (distributionName == "TruncatedDistribution")
  {
    TruncatedDistribution truncatedDistribution = *dynamic_cast<TruncatedDistribution*>(&*inputDistribution.getImplementation());
    Distribution distribution = truncatedDistribution.getDistribution();
    int nbParameters = distribution.getParametersCollection()[0].getSize();
    NumericalPoint newParameters(nbParameters);
    for (int i=0; i<nbParameters; ++i)
      newParameters[i] = parameterValuesEdit_[i]->text().toDouble();
    try
    {
      distribution.setParametersCollection(newParameters);
      truncatedDistribution.setDistribution(distribution);
      physicalModel_.blockNotification(true, "updateProbabilisticModelWindow");
      physicalModel_.setInputDistribution(input.getName(), truncatedDistribution);
      physicalModel_.blockNotification(false);
      updateDistributionWidgets(index);
    }
    catch(Exception)
    {
      std::cerr << "ProbabilisticModelWindow::updateDistribution invalid params:"<<newParameters<<" for distribution:"<<distributionName<<std::endl;
    }
  }
  else
  {
    NumericalPointWithDescription parameters = inputDistribution.getParametersCollection()[0];
    int nbParameters = parameters.getSize();
    if (distributionName == "TruncatedNormal")
      nbParameters = 2;

    for (int i=0; i<nbParameters; ++i)
      parameters[i] = parameterValuesEdit_[i]->text().toDouble();

    try
    {
      inputDistribution.setParametersCollection(parameters);
      physicalModel_.blockNotification(true, "updateProbabilisticModelWindow");
      physicalModel_.setInputDistribution(input.getName(), inputDistribution);
      physicalModel_.blockNotification(false);
      updateDistributionWidgets(index);
    }
    catch(Exception)
    {
      std::cerr << "ProbabilisticModelWindow::updateDistribution invalid params:"<<parameters<<" for distribution:"<<distributionName<<std::endl;
    }
  }
}


void ProbabilisticModelWindow::truncationParametersChanged()
{
  QModelIndex index = inputTableView_->currentIndex();
  Input input = Input(physicalModel_.getInputs()[index.data(Qt::UserRole).toInt()]);
  Distribution inputDistribution = input.getDistribution();
  
  TruncatedDistribution truncatedDistribution = TruncatedDistribution(*dynamic_cast<TruncatedDistribution*>(&*inputDistribution.getImplementation()));

  if (lowerBoundCheckBox_->isChecked())
    truncatedDistribution.setLowerBound(lowerBoundLineEdit_->text().toDouble());
  if (upperBoundCheckBox_->isChecked())
    truncatedDistribution.setUpperBound(upperBoundLineEdit_->text().toDouble());

  physicalModel_.blockNotification(true);
  physicalModel_.setInputDistribution(input.getName(), truncatedDistribution);
  physicalModel_.blockNotification(false);

  updatePlots(truncatedDistribution);
}


void ProbabilisticModelWindow::truncationParametersStateChanged()
{
  QModelIndex index = inputTableView_->currentIndex();
  Input input = Input(physicalModel_.getInputs()[index.data(Qt::UserRole).toInt()]);
  Distribution inputDistribution = input.getDistribution();
  std::string distributionName = inputDistribution.getImplementation()->getClassName();

  if (!lowerBoundCheckBox_->isChecked() && !upperBoundCheckBox_->isChecked())
  {
    lowerBoundLineEdit_->clear();
    upperBoundLineEdit_->clear();
    lowerBoundLineEdit_->setEnabled(false);
    upperBoundLineEdit_->setEnabled(false);
    if (distributionName == "TruncatedDistribution")
    {
      TruncatedDistribution * dist = dynamic_cast<TruncatedDistribution*>(&*inputDistribution.getImplementation());
      inputDistribution = dist->getDistribution();
    }
    else if (distributionName == "TruncatedNormal")
    {
      TruncatedNormal * dist = dynamic_cast<TruncatedNormal*>(&*inputDistribution.getImplementation());
      inputDistribution = Normal(dist->getMu(), dist->getSigma());
    }
    else
      throw;

    physicalModel_.blockNotification(true);
    physicalModel_.setInputDistribution(input.getName(), inputDistribution);
    physicalModel_.blockNotification(false);
    updatePlots(inputDistribution);
  }
  else
  {
    // update widgets
    Interval truncatureInterval = Interval(NumericalPoint(1), NumericalPoint(1), Interval::BoolCollection(1, false), Interval::BoolCollection(1, false));
    Distribution distribution;
    TruncatedDistribution truncatedDistribution;
    if (distributionName == "TruncatedDistribution")
    {
      truncatedDistribution = TruncatedDistribution(*dynamic_cast<TruncatedDistribution*>(&*inputDistribution.getImplementation()));
      distribution = truncatedDistribution.getDistribution();
      truncatureInterval = Interval(truncatedDistribution.getLowerBound(), truncatedDistribution.getUpperBound());
      truncatureInterval.setFiniteLowerBound(Interval::BoolCollection(1, truncatedDistribution.getFiniteLowerBound()));
      truncatureInterval.setFiniteUpperBound(Interval::BoolCollection(1, truncatedDistribution.getFiniteUpperBound()));
    }
    else if (distributionName == "TruncatedNormal")
    {
      TruncatedNormal dist = TruncatedNormal(*dynamic_cast<TruncatedNormal*>(&*inputDistribution.getImplementation()));
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
          truncatureInterval.setLowerBound(NumericalPoint(1, distMean - distStd));
          lowerBoundLineEdit_->blockSignals(true);
          lowerBoundLineEdit_->setText(QString::number(distMean - distStd));
          lowerBoundLineEdit_->blockSignals(false);
          lowerBoundLineEdit_->setEnabled(true);
          truncatureInterval.setFiniteLowerBound(Interval::BoolCollection(1, true));
      }
      else
      {
        lowerBoundLineEdit_->clear();
        lowerBoundLineEdit_->setEnabled(false);
        truncatureInterval.setFiniteLowerBound(Interval::BoolCollection(1, false));
      }
    }
    else if (checkbox == upperBoundCheckBox_)
    {
      if (upperBoundCheckBox_->isChecked())
      {
        double distMean = distribution.getMean()[0];
        double distStd = distribution.getStandardDeviation()[0];
        truncatureInterval.setUpperBound(NumericalPoint(1, distMean + distStd));
        upperBoundLineEdit_->blockSignals(true);
        upperBoundLineEdit_->setText(QString::number(distMean + distStd));
        upperBoundLineEdit_->blockSignals(false);
        upperBoundLineEdit_->setEnabled(true);
        truncatureInterval.setFiniteUpperBound(Interval::BoolCollection(1, true));
      }
      else
      {
        upperBoundLineEdit_->clear();
        upperBoundLineEdit_->setEnabled(false);
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
    physicalModel_.setInputDistribution(input.getName(), truncatedDistribution);
    physicalModel_.blockNotification(false);

    updatePlots(truncatedDistribution);
  }
}
}
