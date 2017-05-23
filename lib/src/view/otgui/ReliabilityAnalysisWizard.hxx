//                                               -*- C++ -*-
/**
 *  @brief QWizard to define a reliability analysis
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
#ifndef OTGUI_RELIABILITYANALYSISWIZARD_HXX
#define OTGUI_RELIABILITYANALYSISWIZARD_HXX

#include "otgui/AnalysisWizard.hxx"
#include "otgui/IntroReliabilityPage.hxx"
#include "otgui/SimulationReliabilityPage.hxx"
#include "otgui/ApproximationReliabilityPage.hxx"
#include "otgui/OptimizationWidget.hxx"
#include "otgui/OTguiItem.hxx"

namespace OTGUI {

// FORM page
class OTGUI_API FORMPage : public QWizardPage
{
  Q_OBJECT

public:
  FORMPage(QWidget* parent=0);

  void initialize(const Analysis& analysis);
  OT::OptimizationSolver getOptimizationAlgorithm() const;

private:
  OptimizationWidget * formWidget_;
};


// ReliabilityAnalysisWizard
class OTGUI_API ReliabilityAnalysisWizard : public AnalysisWizard
{
  Q_OBJECT

public:
  enum {Page_Intro, Page_SimuMethod, Page_ApproxMethod, Page_FORM};

  ReliabilityAnalysisWizard(const OTStudy& otStudy, const LimitState& limitState, QWidget* parent=0);
  ReliabilityAnalysisWizard(OTguiItem* item, const Analysis& analysis, const bool isGeneralWizard=false, QWidget* parent=0);

  virtual int nextId() const;
  virtual bool validateCurrentPage();

protected:
  void buildInterface();

private:
  QList<LimitState> limitStateList_;
  IntroReliabilityPage * introPage_;
  SimulationReliabilityPage * simulationPage_;
  ApproximationReliabilityPage * approximationPage_;
  FORMPage * formPage_;
};
}
#endif