//                                               -*- C++ -*-
/**
 *  @brief Class defining the otgui studies
 *
 *  Copyright 2015-2018 EDF-Phimeca
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
#ifndef OTGUI_OTSTUDYIMPLEMENTATION_HXX
#define OTGUI_OTSTUDYIMPLEMENTATION_HXX

#include "DesignOfExperiment.hxx"
#include "DataModel.hxx"
#include "Analysis.hxx"
#include "LimitState.hxx"
#include "Observer.hxx"

namespace OTGUI
{
class OTGUI_API OTStudyImplementation : public OT::PersistentObject, public Observable, public Observer
{
  CLASSNAME

public:
  /** Default constructor */
  OTStudyImplementation(const OT::String & name = "Unnamed");

  /** Destructor */
  virtual ~OTStudyImplementation();

  /** Virtual constructor */
  virtual OTStudyImplementation * clone() const;

  // Observer method override
  virtual void update(Observable * source, const OT::String & message);

  bool hasBeenModified() const;

  /** Comparison operators */
  OT::Bool operator ==(const OTStudyImplementation & other) const;
  OT::Bool operator !=(const OTStudyImplementation & other) const;

  void clear();

  OT::String getFileName() const;
  void setFileName(const OT::String & fileName);

  OT::Collection<DesignOfExperiment> getDataModels() const;
  DesignOfExperiment & getDataModelByName(const OT::String & dataModelName);
  bool hasDataModelNamed(const OT::String & dataModelName) const;
  OT::String getAvailableDataModelName() const;
  void add(const DesignOfExperiment & designOfExperiment);
  void remove(const DesignOfExperiment & designOfExperiment);

  OT::Collection<PhysicalModel> getPhysicalModels() const;
  PhysicalModel & getPhysicalModelByName(const OT::String & physicalModelName);
  bool hasPhysicalModelNamed(const OT::String & physicalModelName) const;
  OT::String getAvailablePhysicalModelName(const OT::String & physicalModelRootName = "PhysicalModel_") const;
  void add(const PhysicalModel & physicalModel);
  void remove(const PhysicalModel & physicalModel);

  OT::Collection<Analysis> getAnalyses() const;
  Analysis & getAnalysisByName(const OT::String & analysisName);
  bool hasAnalysisNamed(const OT::String & analysisName) const;
  OT::String getAvailableAnalysisName(const OT::String & rootName) const;
  void add(const Analysis & analysis);
  void remove(const Analysis & analysis);

  OT::Collection<LimitState> getLimitStates() const;
  bool hasLimitStateNamed(const OT::String & limitStateName) const;
  OT::String getAvailableLimitStateName() const;
  void add(const LimitState & limitState);
  void remove(const LimitState & limitState);

  OT::String getPythonScript();

  void save(const OT::String & xmlFileName);

  /** Method save() stores the object through the StorageManager */
  void save(OT::Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(OT::Advocate & adv);

private:
  void clear(const DesignOfExperiment & designOfExperiment);
  void clear(const PhysicalModel & physicalModel);
  void clear(const LimitState & limitState);
  void clear(const Analysis & analysis);

protected:
  bool modified_;
  OT::String fileName_;
  OT::PersistentCollection<DesignOfExperiment> dataModels_;
  OT::PersistentCollection<PhysicalModel> physicalModels_;
  OT::PersistentCollection<Analysis> analyses_;
  OT::PersistentCollection<LimitState> limitStates_;
};
}
#endif
