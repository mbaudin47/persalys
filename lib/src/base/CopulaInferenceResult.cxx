//                                               -*- C++ -*-
/**
 *  @brief Copula Inference analysis result
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
#include "otgui/CopulaInferenceResult.hxx"

#include <openturns/PersistentObjectFactory.hxx>

using namespace OT;

namespace OTGUI {

CLASSNAMEINIT(CopulaInferenceResult);

static Factory<CopulaInferenceResult> RegisteredFactory;
static Factory<PersistentCollection<CopulaInferenceSetResult> > RegisteredFactory_CollInfSetResult;

/* Default constructor */
CopulaInferenceResult::CopulaInferenceResult()
  : PersistentObject()
{
}


/* Virtual constructor */
CopulaInferenceResult* CopulaInferenceResult::clone() const
{
  return new CopulaInferenceResult(*this);
}


CopulaInferenceResult::CopulaInferenceSetResultCollection CopulaInferenceResult::getCopulaInferenceSetResultCollection() const
{
  return copulaInferenceSetResultCollection_;
}


CopulaInferenceSetResult CopulaInferenceResult::getCopulaInferenceSetResult(const Description& variablesNames) const
{
  Description setOfVariables(variablesNames);
  setOfVariables.sort();
  for (UnsignedInteger i=0; i<copulaInferenceSetResultCollection_.getSize(); ++i)
  {
    if (copulaInferenceSetResultCollection_[i].getSetOfVariablesNames() == setOfVariables)
      return copulaInferenceSetResultCollection_[i];
  }
  throw InvalidArgumentException(HERE) << "No result for the set of variables " << variablesNames;
}


DesignOfExperiment CopulaInferenceResult::getDesignOfExperiment() const
{
  return designOfExperiment_;
}


/* String converter */
String CopulaInferenceResult::__repr__() const
{
  OSS oss;
  oss << "class=" << GetClassName()
      << " copulaInferenceSetResultCollection=" << getCopulaInferenceSetResultCollection();
  return oss;
}


/* Method save() stores the object through the StorageManager */
void CopulaInferenceResult::save(Advocate& adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute("copulaInferenceSetResultCollection_", copulaInferenceSetResultCollection_);
  adv.saveAttribute("designOfExperiment_", designOfExperiment_);
}


/* Method load() reloads the object from the StorageManager */
void CopulaInferenceResult::load(Advocate& adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute("copulaInferenceSetResultCollection_", copulaInferenceSetResultCollection_);
  adv.loadAttribute("designOfExperiment_", designOfExperiment_);
}
}