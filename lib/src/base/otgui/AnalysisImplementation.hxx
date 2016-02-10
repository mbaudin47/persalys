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
#ifndef OTGUI_ANALYSISIMPLEMENTATION_HXX
#define OTGUI_ANALYSISIMPLEMENTATION_HXX

#include "PhysicalModel.hxx"

namespace OTGUI {
class AnalysisImplementation : public OT::PersistentObject, public Observable
{
public:
  AnalysisImplementation(const OT::String & name, const PhysicalModel & physicalModel);

  virtual AnalysisImplementation * clone() const;

  PhysicalModel getPhysicalModel() const;
  void setPhysicalModel(const PhysicalModel & physicalModel);

  virtual void run();
  virtual OT::String dump() const;
  virtual bool analysisLaunched() const;

  bool isReliabilityAnalysis() const;

protected:
  void setIsReliabilityAnalysis(bool isReliabilityAnalysis);

private:
  PhysicalModel physicalModel_;
  bool isReliabilityAnalysis_;
};
}
#endif