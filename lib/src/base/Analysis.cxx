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
#include "otgui/Analysis.hxx"

using namespace OT;

namespace OTGUI {

CLASSNAMEINIT(Analysis);

Analysis::Analysis(const String & name, const PhysicalModel & physicalModel)
  : TypedInterfaceObject<AnalysisImplementation>(new AnalysisImplementation(name, physicalModel))
{
}

/* Default constructor */
Analysis::Analysis(const AnalysisImplementation & implementation)
  : TypedInterfaceObject<AnalysisImplementation>(implementation.clone())
{
}


/* Constructor from implementation */
Analysis::Analysis(const Implementation & p_implementation)
  : TypedInterfaceObject<AnalysisImplementation>(p_implementation)
{
  // Initialize any other class members here
  // At last, allocate memory space if needed, but go to destructor to free it
}

/* Constructor from implementation pointer */
Analysis::Analysis(AnalysisImplementation * p_implementation)
  : TypedInterfaceObject<AnalysisImplementation>(p_implementation)
{
  // Initialize any other class members here
  // At last, allocate memory space if needed, but go to destructor to free it
}


void Analysis::addObserver(Observer * observer)
{
  getImplementation()->addObserver(observer);
}


PhysicalModel Analysis::getPhysicalModel() const
{
  return getImplementation()->getPhysicalModel();
}


void Analysis::setPhysicalModel(const PhysicalModel & physicalModel)
{
  getImplementation()->setPhysicalModel(physicalModel);
}


bool Analysis::isReliabilityAnalysis() const
{
  return getImplementation()->isReliabilityAnalysis();
}


void Analysis::run()
{
  getImplementation()->run();
}


bool Analysis::analysisLaunched() const
{
  return getImplementation()->analysisLaunched();
}


String Analysis::dump() const
{
  return getImplementation()->dump();
}
}