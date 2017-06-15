//                                               -*- C++ -*-
/**
 *  @brief Base class to define metamodel
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
#ifndef OTGUI_METAMODELANALYSIS_HXX
#define OTGUI_METAMODELANALYSIS_HXX

#include "DesignOfExperimentAnalysis.hxx"
#include "MetaModelAnalysisResult.hxx"

namespace OTGUI {
class OTGUI_API MetaModelAnalysis : public DesignOfExperimentAnalysis
{
public:
  /** Default constructor */
  MetaModelAnalysis();
  /** Constructor with parameters */
  MetaModelAnalysis(const OT::String& name, const DesignOfExperiment& designOfExperiment);

  virtual void setDesignOfExperiment(const DesignOfExperiment& designOfExperiment);
  virtual void setInterestVariables(const OT::Description& variablesNames);

  bool isLeaveOneOutValidation() const;
  void setLeaveOneOutValidation(const bool validation);

  OT::Sample getEffectiveInputSample() const;
  OT::Sample getEffectiveOutputSample() const;

  OT::ComposedDistribution getDistribution();

  /** String converter */
  virtual OT::String __repr__() const;

  /** Method save() stores the object through the StorageManager */
  void save(OT::Advocate& adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(OT::Advocate& adv);

protected:
  virtual OT::Function runAlgo(const OT::Sample& inputSample, const OT::Sample& outputSample) = 0;
  void buildMetaModel(MetaModelAnalysisResult& result, const OT::Function& function);
  void computeError(const OT::Sample& metaOutSample, const OT::Sample& outSample, OT::Point& error, OT::Point& q2);
  void validateMetaModelResult(MetaModelAnalysisResult& result, const OT::Sample& inputSample);

protected:
  OT::ComposedDistribution distribution_;
  bool isDistributionComputed_;
  bool leaveOneOutValidation_;
};
}
#endif