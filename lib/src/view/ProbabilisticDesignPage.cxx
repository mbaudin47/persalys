//                                               -*- C++ -*-
/**
 *  @brief QWizardPage to define a probabilistic design of experiments
 *
 *  Copyright 2015-2020 EDF-Phimeca
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
#include "persalys/ProbabilisticDesignPage.hxx"

#include "persalys/ProbabilisticDesignOfExperiment.hxx"
#include "persalys/CollapsibleGroupBox.hxx"

#include <QGridLayout>
#include <QRadioButton>
#include <QGroupBox>
#include <QLabel>


using namespace OT;

namespace PERSALYS
{

ProbabilisticDesignPage::ProbabilisticDesignPage(QWidget* parent)
  : QWizardPage(parent)
{
  buildInterface();
}


void ProbabilisticDesignPage::buildInterface()
{
  setTitle(tr("Probabilistic design of experiments"));

  QGridLayout * pageLayout = new QGridLayout(this);

  QGroupBox * designGroupBox = new QGroupBox(tr("Designs"));
  QGridLayout * designGroupBoxLayout = new QGridLayout(designGroupBox);
  designsGroup_ = new QButtonGroup(this);

  QRadioButton * buttonToChooseDesign = new QRadioButton(tr("LHS"));
  buttonToChooseDesign->setToolTip(tr("Latin Hypercube Sampling"));
  buttonToChooseDesign->setChecked(true);

  QLabel * cbLabel = new QLabel(tr("Optimisation algorithm"));
  optimComboBox_ = new QComboBox;
  optimComboBox_->addItem(tr("None"), ProbabilisticDesignPage::LHS);
  optimComboBox_->addItem(tr("Simulated Annealing LHS"), ProbabilisticDesignPage::SALHS);
  optimComboBox_->addItem(tr("Monte Carlo LHS"), ProbabilisticDesignPage::MCLHS);

  designsGroup_->addButton(buttonToChooseDesign, optimComboBox_->currentIndex());
  optimComboBox_->setCurrentIndex(designsGroup_->checkedId()>2 ? 0 : designsGroup_->checkedId());
  designGroupBoxLayout->addWidget(buttonToChooseDesign, 0, 0);
  designGroupBoxLayout->addWidget(cbLabel, 0, 1);
  designGroupBoxLayout->addWidget(optimComboBox_, 0, 2);

  mcLhsSizeSB_ = new LogSpinBox;
  mcLhsSizeSB_->setValue(1000);
  designGroupBoxLayout->addWidget( mcLhsSizeSB_, 1, 2);

  connect(buttonToChooseDesign, &QRadioButton::toggled, [=](Bool enabled) {
      optimComboBox_->setEnabled(enabled);
      mcLhsSizeSB_->setEnabled(enabled);});

  connect(optimComboBox_, QOverload<int>::of(&QComboBox::activated), [=](int index) {
      designsGroup_->setId(buttonToChooseDesign,index);
      mcLhsSizeSB_->setEnabled(index == 2);});


  buttonToChooseDesign = new QRadioButton(tr("Monte Carlo"));
  designsGroup_->addButton(buttonToChooseDesign, ProbabilisticDesignPage::MonteCarlo);
  designGroupBoxLayout->addWidget(buttonToChooseDesign, 2, 0);

  buttonToChooseDesign = new QRadioButton(tr("Quasi-Monte Carlo"));
  designsGroup_->addButton(buttonToChooseDesign, ProbabilisticDesignPage::QuasiMonteCarlo);
  designGroupBoxLayout->addWidget(buttonToChooseDesign, 3, 0);

  pageLayout->addWidget(designGroupBox);

  // sample size
  QGroupBox * sampleSizeGroupBox = new QGroupBox(tr("Generation parameter"));
  QGridLayout * sampleSizeLayout = new QGridLayout(sampleSizeGroupBox);

  QLabel * sampleSizeLabel = new QLabel(tr("Sample size"));
  sampleSizeLayout->addWidget(sampleSizeLabel, 0, 0);
  QLabel * sampleTimeLabel = new QLabel(tr("Estimated duration (s):"));
  sampleSizeLayout->addWidget(sampleTimeLabel, 1, 0);

  sampleSizeSpinbox_ = new LogSpinBox;
  sampleSizeSpinbox_->setMinimum(1);
  sampleSizeSpinbox_->setMaximum(std::numeric_limits<int>::max());
  sampleSizeLabel->setBuddy(sampleSizeSpinbox_);
  sampleSizeLayout->addWidget(sampleSizeSpinbox_, 0, 1);
  QLabel * sampleTimeValueLabel = new QLabel(QString::number(sampleSizeSpinbox_->value()*modelEvalTime_));
  sampleSizeLayout->addWidget(sampleTimeValueLabel, 1, 1);
  pageLayout->addWidget(sampleSizeGroupBox);

  sampleTimeValueLabel->setVisible(sampleTimeValueLabel->text().toFloat()>1e-6);
  sampleTimeLabel->setVisible(sampleTimeValueLabel->text().toFloat()>1e-6);

  connect(sampleSizeSpinbox_, QOverload<double>::of(&LogSpinBox::valueChanged), [=](const double& value) {
      sampleTimeValueLabel->setText(QString::number(value*modelEvalTime_));
      if(sampleTimeValueLabel->text().toFloat()>1e-6) {
        sampleTimeValueLabel->setVisible(true);
        emit showTime();}
    });

  connect(this, SIGNAL(showTime()), sampleTimeLabel, SLOT(show()));

  // advanced parameters
  CollapsibleGroupBox * advancedParamGroupBox = new CollapsibleGroupBox;
  advancedParamGroupBox->setTitle(tr("Advanced parameters"));
  QGridLayout * advancedParamGroupBoxLayout = new QGridLayout(advancedParamGroupBox);

  // seed
  QLabel * seedLabel = new QLabel(tr("Seed"));
  advancedParamGroupBoxLayout->addWidget(seedLabel, 0, 0);

  seedSpinbox_ = new QSpinBox;
  seedSpinbox_->setMaximum(std::numeric_limits<int>::max());
  seedLabel->setBuddy(seedSpinbox_);
  advancedParamGroupBoxLayout->addWidget(seedSpinbox_, 0, 1);

  advancedParamGroupBox->setExpanded(false);
  pageLayout->addWidget(advancedParamGroupBox);
  pageLayout->setRowStretch(3, 1);

  initialize(ProbabilisticDesignOfExperiment());
}


void ProbabilisticDesignPage::initialize(const Analysis& analysis)
{
  const DesignOfExperimentEvaluation * doe_ptr = dynamic_cast<const DesignOfExperimentEvaluation*>(analysis.getImplementation().get());

  modelEvalTime_ = doe_ptr->getPhysicalModel().getEvalTime();
  if (!doe_ptr)
    return;

  // check the independence of the copula
  const bool independentCopula = doe_ptr->getPhysicalModel().getCopula().hasIndependentCopula();
  if (!independentCopula)
  {
    designsGroup_->button(ProbabilisticDesignPage::LHS)->setEnabled(false);
    designsGroup_->button(ProbabilisticDesignPage::LHS)->setToolTip(tr("The physical model does not have an independent copula"));
    designsGroup_->button(ProbabilisticDesignPage::MonteCarlo)->click(); // default
  }

  // initialize widgets if the analysis is already a ProbabilisticDesignOfExperiment
  const ProbabilisticDesignOfExperiment * probaDoe_ptr = dynamic_cast<const ProbabilisticDesignOfExperiment*>(doe_ptr);
  if (!probaDoe_ptr)
    return;

  if (probaDoe_ptr->getDesignName() == "LHS" && independentCopula) {
    designsGroup_->button(ProbabilisticDesignPage::LHS)->click();
    optimComboBox_->setCurrentIndex(0);
  }
  else if (probaDoe_ptr->getDesignName() == "SALHS" && independentCopula) {
    designsGroup_->button(ProbabilisticDesignPage::LHS)->click();
    optimComboBox_->setCurrentIndex(1);
  }
  else if (probaDoe_ptr->getDesignName() == "MCLHS" && independentCopula) {
    designsGroup_->button(ProbabilisticDesignPage::LHS)->click();
    optimComboBox_->setCurrentIndex(2);
    mcLhsSizeSB_->setValue(probaDoe_ptr->getMCLHSSize());
  }
  else if (probaDoe_ptr->getDesignName() == "QUASI_MONTE_CARLO")
    designsGroup_->button(ProbabilisticDesignPage::QuasiMonteCarlo)->click();
  else
    designsGroup_->button(ProbabilisticDesignPage::MonteCarlo)->click();

  sampleSizeSpinbox_->setValue(probaDoe_ptr->getSize());
  seedSpinbox_->setValue(probaDoe_ptr->getSeed());
  mcLhsSizeSB_->setEnabled(optimComboBox_->currentIndex()==2);
}

Analysis ProbabilisticDesignPage::getAnalysis(const String& name, const PhysicalModel& model) const
{
  String designType = "LHS";
  if (designsGroup_->checkedId() == ProbabilisticDesignPage::SALHS)
    designType = "SALHS";
  else if (designsGroup_->checkedId() == ProbabilisticDesignPage::MCLHS)
    designType = "MCLHS";
  else if (designsGroup_->checkedId() == ProbabilisticDesignPage::MonteCarlo)
    designType = "MONTE_CARLO";
  else if (designsGroup_->checkedId() == ProbabilisticDesignPage::QuasiMonteCarlo)
    designType = "QUASI_MONTE_CARLO";
  ProbabilisticDesignOfExperiment design(name, model, sampleSizeSpinbox_->value(), designType, mcLhsSizeSB_->isEnabled() ? mcLhsSizeSB_->value() : 0);
  design.setSeed(seedSpinbox_->value());

  return design;
}
}
