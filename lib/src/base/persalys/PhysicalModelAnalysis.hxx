//                                               -*- C++ -*-
/**
 *  @brief Top-level class for all analysis with physical model
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
#ifndef PERSALYS_PHYSICALMODELANALYSIS_HXX
#define PERSALYS_PHYSICALMODELANALYSIS_HXX

#include "AnalysisImplementation.hxx"

namespace PERSALYS
{
class PERSALYS_BASE_API PhysicalModelAnalysis : public AnalysisImplementation
{
public:
  /** Default constructor */
  PhysicalModelAnalysis();

  /** Constructor with parameters */
  PhysicalModelAnalysis(const OT::String & name, const PhysicalModel & physicalModel);

  /** Virtual constructor */
  virtual PhysicalModelAnalysis * clone() const;

  virtual Observer * getParentObserver() const;

  PhysicalModel getPhysicalModel() const;

  virtual bool canBeLaunched(OT::String &errorMessage) const;
  virtual void run();

  /** String converter */
  virtual OT::String __repr__() const;

  /** Method save() stores the object through the StorageManager */
  void save(OT::Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(OT::Advocate & adv);

  virtual void acceptLaunchParameters(LaunchParametersVisitor* visitor);

private:
  PhysicalModel physicalModel_;
};
}
#endif
