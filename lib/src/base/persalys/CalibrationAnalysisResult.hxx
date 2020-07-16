//                                               -*- C++ -*-
/**
 *  @brief Calibration analysis result
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
#ifndef PERSALYS_CALIBRATIONANALYSISRESULT_HXX
#define PERSALYS_CALIBRATIONANALYSISRESULT_HXX

#include "DataSample.hxx"

#include <openturns/CalibrationResult.hxx>

namespace PERSALYS
{

class PERSALYS_BASE_API CalibrationAnalysisResult : public OT::PersistentObject
{
  CLASSNAME

public:
  friend class CalibrationAnalysis;

  /** Default constructor */
  CalibrationAnalysisResult();

  /** Virtual constructor */
  virtual CalibrationAnalysisResult * clone() const;

  OT::CalibrationResult getCalibrationResult() const {return calibrationResult_;};
  OT::Interval getConfidenceInterval() const {return confidenceInterval_;};
  OT::Sample getOutputAtPrior() const {return outputAtPrior_;};
  OT::Sample getOutputAtPosterior() const {return outputAtPosterior_;};
  DataSample::SampleCollection getPriorResidualsPDF() const {return priorResidualsPDF_;};
  DataSample::SampleCollection getPosteriorResidualsPDF() const {return posteriorResidualsPDF_;};

  /** String converter */
  virtual OT::String __repr__() const;

  /** Method save() stores the object through the StorageManager */
  void save(OT::Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(OT::Advocate & adv);

private:
  OT::CalibrationResult calibrationResult_;
  OT::Interval confidenceInterval_;
  OT::Sample outputAtPrior_;
  OT::Sample outputAtPosterior_;
  OT::PersistentCollection<OT::Sample> priorResidualsPDF_;
  OT::PersistentCollection<OT::Sample> posteriorResidualsPDF_;
};
}
#endif
