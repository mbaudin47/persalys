//                                               -*- C++ -*-
/**
 *  @brief YACSEvaluation implements the evaluation of the models defined by an xml file
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
#ifndef PERSALYS_YACSEVALUATION_HXX
#define PERSALYS_YACSEVALUATION_HXX

#include "persalys/PersalysPrivate.hxx"

#include <openturns/EvaluationImplementation.hxx>

#include <ydefx/Launcher.hxx>

namespace PERSALYS
{

/**
 * @class YACSEvaluation
 *
 * The class that implement the evaluation of an analytical function.
 */
class PERSALYS_BASE_API YACSEvaluation : public OT::EvaluationImplementation
{
  CLASSNAME

public:
  /** Default constructor */
  YACSEvaluation(const OT::String & script = "");


  /** Virtual constructor */
  virtual YACSEvaluation * clone() const;

  /** Comparison operator */
  OT::Bool operator ==(const YACSEvaluation & other) const;

  /** String converter */
  virtual OT::String __repr__() const;
  virtual OT::String __str__(const OT::String & offset = "") const;

  /** Operator () */
  virtual OT::Point operator() (const OT::Point & inP) const;
  virtual OT::Sample operator() (const OT::Sample & inS) const;

  /** Accessor for input values */
  OT::Point getInputValues() const;
  void setInputValues(const OT::Point & inP);
  /** Accessor to the input variables names */
  OT::Description getInputVariablesNames() const;
  /** Accessor for input point dimension */
  OT::UnsignedInteger getInputDimension() const;

  /** Accessor to the output variables names */
  OT::Description getOutputVariablesNames() const;
  void setOutputVariablesNames(const OT::Description & outDescription);
  /** Accessor for output point dimension */
  OT::UnsignedInteger getOutputDimension() const;

  /** Accessor to the formulas */
  OT::String getContent() const;
  void setContent(const OT::String & pyScript);

  /** Accessor to launching resource properties */
  ydefx::JobParametersProxy& jobParameters();
  const ydefx::JobParametersProxy& jobParameters()const;

  /** Method save() stores the object through the StorageManager */
  void save(OT::Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(OT::Advocate & adv);

private:
  OT::Point inputValues_;
  OT::Description inDescription_;
  OT::Description outDescription_;
  ydefx::JobParametersProxy jobParams_;
  ydefx::PyStudyFunction studyFunction_;
};
}
#endif
