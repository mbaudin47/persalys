//                                               -*- C++ -*-
/**
 *  @brief SobolResult contains the Sobol indices
 *
 *  Copyright 2015-2016 EDF-Phimeca
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
#include "otgui/SobolResult.hxx"

#include "openturns/PersistentObjectFactory.hxx"

using namespace OT;

namespace OTGUI {

CLASSNAMEINIT(SobolResult);

static Factory<SobolResult> RegisteredFactory;

/* Default constructor */
SobolResult::SobolResult()
  : PersistentObject()
  , callsNumber_(0)
  , elapsedTime_(0.)
  , coefficientOfVariation_(-1.)
{
}


/* Constructor with parameters */
SobolResult::SobolResult(const NumericalSample firstOrderIndices,
                         const NumericalSample totalIndices,
                         const Description & outputNames)
  : PersistentObject()
  , outputNames_(outputNames)
  , firstOrderIndices_(firstOrderIndices)
  , totalIndices_(totalIndices)
  , callsNumber_(0)
  , elapsedTime_(0.)
  , coefficientOfVariation_(-1.)
{
}


/* Virtual constructor */
SobolResult* SobolResult::clone() const
{
  return new SobolResult(*this);
}


Description SobolResult::getOutputNames() const
{
  return outputNames_;
}


Description SobolResult::getInputNames() const
{
  return firstOrderIndices_.getDescription();
}


NumericalSample SobolResult::getFirstOrderIndices() const
{
  return firstOrderIndices_;
}


NumericalSample SobolResult::getTotalIndices() const
{
  return totalIndices_;
}


UnsignedInteger SobolResult::getCallsNumber() const
{
  return callsNumber_;
}


void SobolResult::setCallsNumber(const OT::UnsignedInteger number)
{
  callsNumber_ = number;
}


double SobolResult::getElapsedTime() const
{
  return elapsedTime_;
}


void SobolResult::setElapsedTime(const double seconds)
{
  elapsedTime_ = seconds;
}


double SobolResult::getCoefficientOfVariation() const
{
  return coefficientOfVariation_;
}


void SobolResult::setCoefficientOfVariation(const double coef)
{
  coefficientOfVariation_ = coef;
}


/* String converter */
String SobolResult::__repr__() const
{
  OSS oss;
  oss << "class=" << GetClassName()
      << " name=" << getName()
      << " outputNames=" << getOutputNames()
      << " firstOrderIndices=" << getFirstOrderIndices()
      << " totalIndices=" << getTotalIndices()
      << " callsNumber=" << getCallsNumber()
      << " coefficientOfVariation=" << getCoefficientOfVariation();
  return oss;
}


/* Method save() stores the object through the StorageManager */
void SobolResult::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute("outputNames_", outputNames_);
  adv.saveAttribute("firstOrderIndices_", firstOrderIndices_);
  adv.saveAttribute("totalIndices_", totalIndices_);
  adv.saveAttribute("elapsedTime_", elapsedTime_);
  adv.saveAttribute("callsNumber_", callsNumber_);
  adv.saveAttribute("coefficientOfVariation_", coefficientOfVariation_);
}


/* Method load() reloads the object from the StorageManager */
void SobolResult::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute("outputNames_", outputNames_);
  adv.loadAttribute("firstOrderIndices_", firstOrderIndices_);
  adv.loadAttribute("totalIndices_", totalIndices_);
  adv.loadAttribute("elapsedTime_", elapsedTime_);
  adv.loadAttribute("callsNumber_", callsNumber_);
  adv.loadAttribute("coefficientOfVariation_", coefficientOfVariation_);
}
}