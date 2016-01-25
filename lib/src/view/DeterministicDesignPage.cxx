// DeterministicDesignPage.cxx

#include "otgui/DeterministicDesignPage.hxx"

#include "otgui/ExperimentTableModel.hxx"
#include "otgui/HorizontalHeaderViewWithCombobox.hxx"

#include <NumericalSample.hxx>

#include <QVBoxLayout>
#include <QRadioButton>

using namespace OT;

namespace OTGUI {

DeterministicDesignPage::DeterministicDesignPage(const DesignOfExperiment & designOfExperiment, QWidget* parent)
  : QWizardPage(parent)
  , designOfExperiment_(designOfExperiment)
  , pageValidity_(true)
{
  designOfExperiment_.updateParameters();
  buildInterface();
}

void DeterministicDesignPage::buildInterface()
{
  setTitle(tr("Deterministic design of experiment"));

  QVBoxLayout * pageLayout = new QVBoxLayout(this);

  methodGroup_ = new QButtonGroup;
  QRadioButton * buttonToChooseMethod = new QRadioButton(tr("Define manually"));
  buttonToChooseMethod->setChecked(true);
  methodGroup_->addButton(buttonToChooseMethod, DesignOfExperimentImplementation::FromBoundsAndLevels);
  pageLayout->addWidget(buttonToChooseMethod);

  tableView_ = new QTableView;
  tableView_->setEditTriggers(QTableView::AllEditTriggers);
  tableView_->setEnabled(true);
  if (designOfExperiment_.getTypeDesignOfExperiment() == DesignOfExperimentImplementation::FromExperiment)
    tableView_->setEnabled(false);

  ExperimentTableModel * model = new ExperimentTableModel(designOfExperiment_);
  connect(model, SIGNAL(designOfExperimentChanged(DesignOfExperiment &)), this, SLOT(setDesignOfExperiment(DesignOfExperiment &)));
  tableView_->setModel(model);

  QStringList items = QStringList()<<tr("Levels")<<tr("Delta");
  QVector<int> columns(1, 6);
  HorizontalHeaderViewWithCombobox * header = new HorizontalHeaderViewWithCombobox(items, columns, tableView_);
  tableView_->setHorizontalHeader(header);
  tableView_->horizontalHeader()->show();

  pageLayout->addWidget(tableView_);

  // TODO
//   buttonToChooseMethod = new QRadioButton(tr("Use a stratified design"));
//   methodGroup_->addButton(buttonToChooseMethod, DesignOfExperimentImplementation::FromExperiment);
//   pageLayout->addWidget(buttonToChooseMethod);
// 
//   QButtonGroup * designsGroup_ = new QButtonGroup;
//   designsWidget_ = new QWidget;
//   QVBoxLayout * designsLayout = new QVBoxLayout(designsWidget_);
//   QRadioButton * buttonToChooseDesign = new QRadioButton(tr("Axial"));
//   buttonToChooseDesign->setChecked(true);
//   designsGroup_->addButton(buttonToChooseDesign);
//   designsLayout->addWidget(buttonToChooseDesign);
// 
//   buttonToChooseDesign = new QRadioButton(tr("Factorial"));
//   designsGroup_->addButton(buttonToChooseDesign);
//   designsLayout->addWidget(buttonToChooseDesign);
// 
//   buttonToChooseDesign = new QRadioButton(tr("Composite"));
//   designsGroup_->addButton(buttonToChooseDesign);
//   designsLayout->addWidget(buttonToChooseDesign);
// 
//   designsWidget_->setEnabled(true);
//   if (designOfExperiment_.getTypeDesignOfExperiment() != DesignOfExperimentImplementation::FromExperiment)
//     designsWidget_->setEnabled(false);
//   pageLayout->addWidget(designsWidget_);

  connect(methodGroup_, SIGNAL(buttonClicked(int)), this, SLOT(setEnabledWidgets(int)));
}


void DeterministicDesignPage::setEnabledWidgets(int method)
{
  switch (methodGroup_->checkedId())
  {
    case DesignOfExperimentImplementation::FromBoundsAndLevels:
    {
      tableView_->setEnabled(true);
//       TODO
//       designsWidget_->setEnabled(false);
//       designOfExperiment_ = DesignOfExperiment(designOfExperiment_.getName(),
//                                                designOfExperiment_.getPhysicalModel(),
//                                                Bounds
//                                               );
//       emit designOfExperimentChanged(designOfExperiment_);
      break;
    }
//     case DesignOfExperimentImplementation::FromExperiment:
//     {
//       tableView_->setEnabled(false);
//       designsWidget_->setEnabled(true);
//       TODO
//       designOfExperiment_ = DesignOfExperiment(designOfExperiment_.getName(),
//                                                designOfExperiment_.getPhysicalModel(),
//                                                Experiment
//                                               );
//       emit designOfExperimentChanged(designOfExperiment_);
//       break;
//     }
    default:
      break;
  }
}


void DeterministicDesignPage::setDesignOfExperiment(DesignOfExperiment & designOfExperiment)
{
  designOfExperiment_ = designOfExperiment;
}


bool DeterministicDesignPage::validatePage()
{
  return pageValidity_;
}
}