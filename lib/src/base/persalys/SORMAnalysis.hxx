//                                               -*- C++ -*-
/**
 *  @brief Analysis SORM
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
#ifndef PERSALYS_SORMANALYSIS_HXX
#define PERSALYS_SORMANALYSIS_HXX

#include "ApproximationAnalysis.hxx"
#include "ReliabilityAnalysis.hxx"
#include "SORMAnalysisResult.hxx"

namespace PERSALYS
{
class PERSALYS_BASE_API SORMAnalysis : public ReliabilityAnalysis, public ApproximationAnalysis
{
  CLASSNAME

public:
  /** Default constructor */
  SORMAnalysis();
  /** Constructor with parameters */
  SORMAnalysis(const OT::String& name, const LimitState& limitState);

  /** Virtual constructor */
  virtual SORMAnalysis * clone() const;

  SORMAnalysisResult getResult() const;

  virtual Parameters getParameters() const;
  virtual OT::String getPythonScript() const;
  virtual bool hasValidResult() const;

  /** String converter */
  virtual OT::String __repr__() const;

  /** Method save() stores the object through the StorageManager */
  void save(OT::Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(OT::Advocate & adv);

protected:
  virtual void initialize();
  virtual void launch();

private:
  SORMAnalysisResult result_;
};
}
#endif
