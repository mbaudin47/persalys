//                                               -*- C++ -*-
/**
 *  @brief QWizardPage to define the type of design of experiments
 *
 *  Copyright 2015-2018 EDF-Phimeca
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
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "otgui/DesignOfExperimentIntroPage.hxx"

#include "otgui/DesignOfExperimentEvaluation.hxx"
#include "otgui/DesignOfExperimentWizard.hxx"

#include <QVBoxLayout>
#include <QGroupBox>
#include <QRadioButton>

using namespace OT;

namespace OTGUI
{

DesignOfExperimentIntroPage::DesignOfExperimentIntroPage(QWidget* parent)
  : QWizardPage(parent)
  , methodGroup_(0)
{
  setTitle(tr("Input sample definition"));

  QVBoxLayout * pageLayout = new QVBoxLayout(this);

  QGroupBox * methodBox = new QGroupBox(tr("Type of design of experiments"));
  QVBoxLayout * methodLayout = new QVBoxLayout(methodBox);

  methodGroup_ = new QButtonGroup(this);

  // Deterministic
  QRadioButton * deterministicButton = new QRadioButton(tr("Deterministic"));
  methodGroup_->addButton(deterministicButton, DesignOfExperimentIntroPage::deterministic);
  methodLayout->addWidget(deterministicButton);

  // Probabilistic
  QRadioButton * probaButton = new QRadioButton(tr("Probabilistic"));
  methodGroup_->addButton(probaButton, DesignOfExperimentIntroPage::probabilistic);
  methodLayout->addWidget(probaButton);

  // import
  QRadioButton * importButton = new QRadioButton(tr("Import data"));
  methodGroup_->addButton(importButton, DesignOfExperimentIntroPage::import);
  methodLayout->addWidget(importButton);

  pageLayout->addWidget(methodBox);

  // register fields
  registerField("deterministicButton", deterministicButton);
  registerField("probaButton", probaButton);
  registerField("importButton", importButton);
}


void DesignOfExperimentIntroPage::initialize(const Analysis& analysis)
{
  const DesignOfExperimentEvaluation * analysis_ptr = dynamic_cast<const DesignOfExperimentEvaluation*>(analysis.getImplementation().get());
  if (!analysis_ptr)
    return;

  if (!analysis_ptr->getPhysicalModel().hasStochasticInputs())
  {
    methodGroup_->button(DesignOfExperimentIntroPage::probabilistic)->setToolTip(tr("The physical model has no stochastic inputs"));
    methodGroup_->button(DesignOfExperimentIntroPage::probabilistic)->setEnabled(false);
  }

  // method
  const String analysisName = analysis.getImplementation()->getClassName();

  if (analysisName == "ProbabilisticDesignOfExperiment" && analysis_ptr->getPhysicalModel().hasStochasticInputs())
    methodGroup_->button(DesignOfExperimentIntroPage::probabilistic)->click();
  else if (analysisName == "ImportedDesignOfExperiment")
    methodGroup_->button(DesignOfExperimentIntroPage::import)->click();
  else
    methodGroup_->button(DesignOfExperimentIntroPage::deterministic)->click();
}


int DesignOfExperimentIntroPage::nextId() const
{
  switch (DesignOfExperimentIntroPage::Method(methodGroup_->checkedId()))
  {
    case DesignOfExperimentIntroPage::deterministic:
    {
      return DesignOfExperimentWizard::Page_Deterministic;
    }
    case DesignOfExperimentIntroPage::probabilistic:
    {
      return DesignOfExperimentWizard::Page_Probabilistic;
    }
    case DesignOfExperimentIntroPage::import:
    {
      return DesignOfExperimentWizard::Page_Import;
    }
    default:
      return -1;
      break;
  }
}
}