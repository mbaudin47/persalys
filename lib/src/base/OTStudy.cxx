//                                               -*- C++ -*-
/**
 *  @brief Class defining the otgui studies
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
#include "otgui/OTStudy.hxx"

#include "otgui/ReliabilityAnalysis.hxx"

using namespace OT;

namespace OTGUI {

std::vector<OTStudy*> OTStudy::OTStudies_;
Observer * OTStudy::OTStudyObserver_ = 0;


std::vector<OTStudy*> OTStudy::GetOTStudies()
{
  return OTStudies_;
}


OTStudy * OTStudy::GetOTStudyByName(const String & otStudyName)
{
  for (std::vector<OTStudy*>::iterator it=OTStudies_.begin(); it!=OTStudies_.end(); ++it)
    if ((*it)->getName() == otStudyName)
      return (*it);
  return 0;
}


bool OTStudy::HasOTStudyNamed(const String & otStudyName)
{
  for (std::vector<OTStudy*>::iterator it=OTStudies_.begin(); it!=OTStudies_.end(); ++it)
    if ((*it)->getName() == otStudyName)
      return true;
  return false;
}


String OTStudy::GetAvailableOTStudyName()
{
  int i = 0;
  String rootName = "OTStudy_";
  while (HasOTStudyNamed(rootName + (OSS()<<i).str()))
    ++i;
  return rootName + (OSS()<<i).str();
}


void OTStudy::SetInstanceObserver(Observer * observer)
{
  OTStudyObserver_ = observer;
}


OTStudy::OTStudy(const String & name)
  : PersistentObject()
  , Observable()
{
  setName(name);
  OTStudies_.push_back(this);
  if (OTStudyObserver_)
    OTStudyObserver_->update(this, "addStudy");
}


OTStudy::OTStudy(const OTStudy & other)
  : PersistentObject()
  , Observable()
{
  setName(other.getName()+"_copy");
  setObserver(other.getObserver());
  OTStudies_.push_back(this);
  if (OTStudyObserver_)
    OTStudyObserver_->update(this, "addStudy");

  for (int i=0; i<other.physicalModels_.size(); ++i)
    addPhysicalModel(*other.physicalModels_[i].getImplementation()->clone());
  // TODO copy of designOfExperiments_/analyses_/limitStates_
}


OTStudy* OTStudy::clone() const
{
  return new OTStudy(*this);
}


OTStudy::~OTStudy()
{
  OTStudies_.erase(std::remove(OTStudies_.begin(), OTStudies_.end(), this), OTStudies_.end());
}


std::vector<PhysicalModel> OTStudy::getPhysicalModels() const
{
  return physicalModels_;
}


PhysicalModel & OTStudy::getPhysicalModelByName(const String & physicalModelName)
{
  for (UnsignedInteger i=0; i<physicalModels_.size(); ++i)
    if (physicalModels_[i].getName() == physicalModelName)
      return physicalModels_[i];
  throw InvalidArgumentException(HERE) << "The given name " << physicalModelName <<" does not correspond to a physical model of the study.\n"; 
}


bool OTStudy::hasPhysicalModelNamed(const String & physicalModelName)
{
  for (UnsignedInteger i=0; i<physicalModels_.size(); ++i)
    if (physicalModels_[i].getImplementation()->getName() == physicalModelName)
      return true;
  return false;
}


String OTStudy::getAvailablePhysicalModelName()
{
  int i = 0;
  String rootName = "physicalModel_";
  while (hasPhysicalModelNamed(rootName + (OSS()<<i).str()))
    ++i;
  return rootName + (OSS()<<i).str();
}


void OTStudy::addPhysicalModel(const PhysicalModel & physicalModel)
{
  if (hasPhysicalModelNamed(physicalModel.getName()))
    throw InvalidArgumentException(HERE) << "The study has already contained a physical model named " << physicalModel.getName();

  physicalModels_.push_back(physicalModel);
  notify("addPhysicalModel");
}


std::vector<DesignOfExperiment> OTStudy::getDesignOfExperiments() const
{
  return designOfExperiments_;
}


bool OTStudy::hasDesignOfExperimentNamed(const String & designOfExperimentName)
{
  for (UnsignedInteger i=0; i<designOfExperiments_.size(); ++i)
    if (designOfExperiments_[i].getImplementation()->getName() == designOfExperimentName)
      return true;
  return false;
}


String OTStudy::getAvailableDesignOfExperimentName()
{
  int i = 0;
  String rootName = "design_";
  while (hasDesignOfExperimentNamed(rootName + (OSS()<<i).str()))
    ++i;
  return rootName + (OSS()<<i).str();
}


void OTStudy::addDesignOfExperiment(const DesignOfExperiment & designOfExperiment)
{
  if (hasDesignOfExperimentNamed(designOfExperiment.getName()))
    throw InvalidArgumentException(HERE) << "The study has already contained a design of experiment named " << designOfExperiment.getName();

  if (!hasPhysicalModelNamed(designOfExperiment.getPhysicalModel().getName()))
    throw InvalidArgumentException(HERE) << "The design of experiment has been created with a physical model not belonging to the study.";

  designOfExperiments_.push_back(designOfExperiment);
  notify("addDesignOfExperiment");
}


std::vector<Analysis> OTStudy::getAnalyses() const
{
  return analyses_;
}

Analysis & OTStudy::getAnalysisByName(const String & analysisName)
{
  for (UnsignedInteger i=0; i<analyses_.size(); ++i)
    if (analyses_[i].getName() == analysisName)
      return analyses_[i];
  throw InvalidArgumentException(HERE) << "The given name " << analysisName <<" does not correspond to an analysis of the study.\n"; 
}


bool OTStudy::hasAnalysisNamed(const String & analysisName)
{
  for (UnsignedInteger i=0; i<analyses_.size(); ++i)
    if (analyses_[i].getImplementation()->getName() == analysisName)
      return true;
  return false;
}


String OTStudy::getAvailableAnalysisName(const String & rootName)
{
  int i = 0;
  while (hasAnalysisNamed(rootName + (OSS()<<i).str()))
    ++i;
  return rootName + (OSS()<<i).str();
}


void OTStudy::addAnalysis(const Analysis & analysis)
{
  if (hasAnalysisNamed(analysis.getName()))
    throw InvalidArgumentException(HERE) << "The study has already contained an analysis named " << analysis.getName();

  if (!hasPhysicalModelNamed(analysis.getPhysicalModel().getName()))
    throw InvalidArgumentException(HERE) << "The analysis has been created with a physical model not belonging to the study.";

  if (analysis.isReliabilityAnalysis())
    if (!hasLimitStateNamed(dynamic_cast<const ReliabilityAnalysis*>(&*analysis.getImplementation())->getLimitState().getName()))
      throw InvalidArgumentException(HERE) << "The analysis has been created with a limit state not belonging to the study.";

  analyses_.push_back(analysis);
  notify("add"+analysis.getImplementation()->getClassName());
}


std::vector<LimitState> OTStudy::getLimitStates() const
{
  return limitStates_;
}


bool OTStudy::hasLimitStateNamed(const String & limitStateName)
{
  for (UnsignedInteger i=0; i<limitStates_.size(); ++i)
    if (limitStates_[i].getImplementation()->getName() == limitStateName)
      return true;
  return false;
}


String OTStudy::getAvailableLimitStateName()
{
  int i = 0;
  String rootName = "limitState_";
  while (hasLimitStateNamed(rootName + (OSS()<<i).str()))
    ++i;
  return rootName + (OSS()<<i).str();
}


void OTStudy::addLimitState(const LimitState & limitState)
{
  if (hasLimitStateNamed(limitState.getName()))
    throw InvalidArgumentException(HERE) << "The study has already contained a limit state named " << limitState.getName();

  if (!hasPhysicalModelNamed(limitState.getPhysicalModel().getName()))
    throw InvalidArgumentException(HERE) << "The limit state has been created with a physical model not belonging to the study.";

  limitStates_.push_back(limitState);
  notify("addLimitState");
}


String OTStudy::dump()
{
  String result;
  result += getName()+ " = otguibase.OTStudy('" + getName() + "')\n";
  for (std::vector<PhysicalModel>::iterator it=physicalModels_.begin(); it!= physicalModels_.end(); ++it)
  {
    result += (*it).dump();
    result += getName() + ".addPhysicalModel(" + (*it).getName() + ")\n";
  }
  for (std::vector<DesignOfExperiment>::iterator it=designOfExperiments_.begin(); it!= designOfExperiments_.end(); ++it)
  {
    result += (*it).dump();
    result += getName() + ".addDesignOfExperiment(" + (*it).getName() + ")\n";
  }
  for (std::vector<LimitState>::iterator it=limitStates_.begin(); it!= limitStates_.end(); ++it)
  {
    result += (*it).dump();
    result += getName() + ".addLimitState(" + (*it).getName() + ")\n";
  }
  for (std::vector<Analysis>::iterator it=analyses_.begin(); it!= analyses_.end(); ++it)
  {
    result += (*it).dump();
    result += getName() + ".addAnalysis(" + (*it).getName() + ")\n";
  }
  return result;
}
}