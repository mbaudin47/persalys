// DesignOfExperimentWizard.cxx

#include "otgui/DesignOfExperimentWizard.hxx"

#include "otgui/ImportTablePage.hxx"
#include "otgui/DeterministicDesignPage.hxx"
#include "otgui/ProbabilisticDesignPage.hxx"

#include <QVBoxLayout>
#include <QTableView>
#include <QGroupBox>
#include <QRadioButton>

namespace OTGUI {

IntroPage::IntroPage(bool physicalModelHasStochasticInputs, QWidget *parent)
  : QWizardPage(parent)
{
  setTitle(tr("Introduction"));

  QVBoxLayout * pageLayout = new QVBoxLayout(this);

  QGroupBox * methodBox = new QGroupBox(tr("Type of design of experiment"));
  QVBoxLayout * methodLayout = new QVBoxLayout(methodBox);

  methodGroup_ = new QButtonGroup;
  QRadioButton * buttonToChooseMethod = new QRadioButton(tr("Deterministic"));
  buttonToChooseMethod->setChecked(true);
  methodGroup_->addButton(buttonToChooseMethod, IntroPage::deterministic);
  methodLayout->addWidget(buttonToChooseMethod);
//   TODO
  buttonToChooseMethod = new QRadioButton(tr("Probabilistic"));
  buttonToChooseMethod->setEnabled(physicalModelHasStochasticInputs);
  methodGroup_->addButton(buttonToChooseMethod, IntroPage::probabilistic);
  methodLayout->addWidget(buttonToChooseMethod);
  buttonToChooseMethod = new QRadioButton(tr("Import data"));
  methodGroup_->addButton(buttonToChooseMethod, IntroPage::import);
  methodLayout->addWidget(buttonToChooseMethod);

  pageLayout->addWidget(methodBox);
}


int IntroPage::nextId() const
{
  switch (IntroPage::Method(methodGroup_->checkedId()))
  {
    case IntroPage::deterministic:
    {
      return DesignOfExperimentWizard::Page_Deterministic;
    }
//     TODO
    case IntroPage::probabilistic:
    {
      return DesignOfExperimentWizard::Page_Probabilistic;
    }
    case IntroPage::import:
    {
      return DesignOfExperimentWizard::Page_Import;
    }
    default:
      break;
  }
}


DesignOfExperimentWizard::DesignOfExperimentWizard(OTStudy * otStudy, const PhysicalModel & physicalModel)
  : QWizard()
  , designOfExperiment_(DesignOfExperiment("aDesign", physicalModel))
  , otStudy_(otStudy)
{
  buildInterface();
}


DesignOfExperimentWizard::DesignOfExperimentWizard(const DesignOfExperiment & designOfExperiment)
  : QWizard()
  , designOfExperiment_(designOfExperiment)
{
  buildInterface();
}


void DesignOfExperimentWizard::buildInterface()
{
  setWindowTitle("Design Of Experiment");

  introPage_ = new IntroPage(designOfExperiment_.getPhysicalModel().hasStochasticInputs());
  setPage(Page_Intro, introPage_);
  setPage(Page_Deterministic, new DeterministicDesignPage(designOfExperiment_));
  setPage(Page_Probabilistic, new ProbabilisticDesignPage(designOfExperiment_));
  setPage(Page_Import, new ImportTablePage(designOfExperiment_));

  setStartId(Page_Intro);
}

int DesignOfExperimentWizard::nextId() const
{
  switch (currentId())
  {
    case Page_Intro:
      return introPage_->nextId();
    case Page_Deterministic:
    case Page_Probabilistic:
    case Page_Import:
    default:
        return -1;
  }
}


void DesignOfExperimentWizard::setDesignOfExperiment(DesignOfExperiment & designOfExperiment)
{
  designOfExperiment_ = designOfExperiment;
}


void DesignOfExperimentWizard::validate()
{
  otStudy_->addDesignOfExperiment(designOfExperiment_);
}
}