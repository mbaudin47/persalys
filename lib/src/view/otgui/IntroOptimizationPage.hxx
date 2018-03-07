//                                               -*- C++ -*-
/**
 *  @brief QWizardPage to define the method of optimization
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
#ifndef OTGUI_INTROOPTIMIZATIONPAGE_HXX
#define OTGUI_INTROOPTIMIZATIONPAGE_HXX

#include "otgui/Analysis.hxx"
#include "otgui/OutputsSelectionGroupBox.hxx"
#include "otgui/TemporaryLabel.hxx"

#include <QWizardPage>
#include <QButtonGroup>

namespace OTGUI
{

class OTGUI_API IntroOptimizationPage : public QWizardPage
{
  Q_OBJECT

public:
  enum OptimAlgo {AbdoRackwitzAlgo, CobylaAlgo, SQPAlgo, NLoptAlgo};

  IntroOptimizationPage(QWidget* parent = 0);

  void initialize(const Analysis& analysis);

  OT::Description getInterestVariables() const;

  OT::String getSolverName() const;

  virtual bool validatePage();

protected slots:
  void openUrl();

private:
  OutputsSelectionGroupBox * outputsSelectionGroupBox_;
  QButtonGroup * methodGroup_;
  TemporaryLabel * errorMessageLabel_;
};
}
#endif