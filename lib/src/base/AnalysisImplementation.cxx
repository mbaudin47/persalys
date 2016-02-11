//                                               -*- C++ -*-
/**
 *  @brief Abstract top-level class for all analysis 
 *
 *  Copyright 2015-2016 EDF
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
#include "otgui/AnalysisImplementation.hxx"

using namespace OT;

namespace OTGUI {

AnalysisImplementation::AnalysisImplementation(const String & name, const PhysicalModel & physicalModel)
  : PersistentObject()
  , Observable()
  , physicalModel_(physicalModel)
  , isReliabilityAnalysis_(false)
{
  setName(name);
}


AnalysisImplementation* AnalysisImplementation::clone() const
{
  return new AnalysisImplementation(*this);
}


PhysicalModel AnalysisImplementation::getPhysicalModel() const
{
  return physicalModel_;
}


void AnalysisImplementation::setPhysicalModel(const PhysicalModel & physicalModel)
{
  physicalModel_ = physicalModel;
}


bool AnalysisImplementation::isReliabilityAnalysis() const
{
  return isReliabilityAnalysis_;
}


void AnalysisImplementation::setIsReliabilityAnalysis(bool isReliabilityAnalysis)
{
  isReliabilityAnalysis_ = isReliabilityAnalysis;
}


void AnalysisImplementation::run()
{

}


bool AnalysisImplementation::analysisLaunched() const
{
  return false;
}


String AnalysisImplementation::dump() const
{
  return "";
}


/* Method save() stores the object through the StorageManager */
void AnalysisImplementation::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute("physicalModel_", physicalModel_);
  adv.saveAttribute("isReliabilityAnalysis_", isReliabilityAnalysis_);
}


/* Method load() reloads the object from the StorageManager */
void AnalysisImplementation::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute("physicalModel_", physicalModel_);
  adv.loadAttribute("isReliabilityAnalysis_", isReliabilityAnalysis_);
}
}