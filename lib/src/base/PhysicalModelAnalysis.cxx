//                                               -*- C++ -*-
/**
 *  @brief Top-level class for all analysis with physical model
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
#include "otgui/PhysicalModelAnalysis.hxx"

using namespace OT;

namespace OTGUI {

/* Default constructor */
PhysicalModelAnalysis::PhysicalModelAnalysis()
  : AnalysisImplementation()
{
}


/* Constructor with parameters */
PhysicalModelAnalysis::PhysicalModelAnalysis(const String & name, const PhysicalModel & physicalModel)
  : AnalysisImplementation(name)
  , physicalModel_(physicalModel)
{
  setInterestVariables(physicalModel.getSelectedOutputsNames());
}


/* Virtual constructor */
PhysicalModelAnalysis* PhysicalModelAnalysis::clone() const
{
  return new PhysicalModelAnalysis(*this);
}


PhysicalModel PhysicalModelAnalysis::getPhysicalModel() const
{
  return physicalModel_;
}


String PhysicalModelAnalysis::getModelName() const
{
  return physicalModel_.getName();
}


void PhysicalModelAnalysis::setInterestVariables(const Description& outputsNames)
{
  if (!outputsNames.getSize())
    throw InvalidDimensionException(HERE) << "The number of outputs to analyse must be superior to 0";

  const Description modelOutputsNames(physicalModel_.getOutputNames());
  for (UnsignedInteger i=0; i<outputsNames.getSize(); ++i)
    if (!modelOutputsNames.contains(outputsNames[i]))
      throw InvalidArgumentException(HERE) << "The name " << outputsNames[i] << " does not match an output name of the model";

  AnalysisImplementation::setInterestVariables(outputsNames);
}


String PhysicalModelAnalysis::__repr__() const
{
  OSS oss;
  oss << "class=" << getClassName()
      << " name=" << getName()
      << " physicalModel=" << getPhysicalModel().getName()
      << " interestVariables=" << getInterestVariables();
  return oss;
}


/* Method save() stores the object through the StorageManager */
void PhysicalModelAnalysis::save(Advocate & adv) const
{
  AnalysisImplementation::save(adv);
  adv.saveAttribute("physicalModel_", physicalModel_);
}


/* Method load() reloads the object from the StorageManager */
void PhysicalModelAnalysis::load(Advocate & adv)
{
  AnalysisImplementation::load(adv);
  adv.loadAttribute("physicalModel_", physicalModel_);
  if (!getInterestVariables().getSize() && !isReliabilityAnalysis())
    setInterestVariables(physicalModel_.getSelectedOutputsNames());
}
}