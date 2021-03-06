//                                               -*- C++ -*-
/**
 *  @brief Evaluates the design of experiments
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
#ifndef PERSALYS_DESIGNOFEXPERIMENTEVALUATION_HXX
#define PERSALYS_DESIGNOFEXPERIMENTEVALUATION_HXX

#include "SimulationAnalysis.hxx"
#include "DataAnalysisResult.hxx"

namespace PERSALYS
{
class PERSALYS_BASE_API DesignOfExperimentEvaluation : public SimulationAnalysis
{
  CLASSNAME

public:
  /** Default constructor */
  DesignOfExperimentEvaluation();

  /** Constructor with parameters */
  DesignOfExperimentEvaluation(const OT::String& name, const PhysicalModel& physicalModel);

  /** Virtual constructor */
  virtual DesignOfExperimentEvaluation * clone() const;

  /** Object name accessor */
  virtual void setName(const OT::String& name);

  virtual void removeAllObservers();

  virtual OT::Sample getOriginalInputSample() const;

  OT::Sample getNotEvaluatedInputSample() const;

  void setDesignOfExperiment(const DesignOfExperiment& designOfExperiment);
  DataAnalysisResult getResult() const;

  virtual Parameters getParameters() const;
  virtual bool hasValidResult() const;
  virtual bool canBeLaunched(OT::String &errorMessage) const;

  /** Method save() stores the object through the StorageManager */
  void save(OT::Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(OT::Advocate & adv);

protected:
  virtual void initialize();
  virtual void launch();

protected:
  mutable OT::Sample originalInputSample_;
  DataAnalysisResult result_;
};
}
#endif
