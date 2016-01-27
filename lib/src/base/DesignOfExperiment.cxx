// DesignOfExperiment.cxx

#include "otgui/DesignOfExperiment.hxx"

using namespace OT;

namespace OTGUI {

CLASSNAMEINIT(DesignOfExperiment);

DesignOfExperiment::DesignOfExperiment(const String & name, const PhysicalModel & physicalModel)
  : TypedInterfaceObject<DesignOfExperimentImplementation>(new DesignOfExperimentImplementation(name, physicalModel))
{
}


DesignOfExperiment::DesignOfExperiment(const String & name, const PhysicalModel & physicalModel,
                                       const NumericalPoint & lowerBounds,
                                       const NumericalPoint & upperBounds, const Indices & nbValues,
                                       const OT::NumericalPoint & values)
  : TypedInterfaceObject<DesignOfExperimentImplementation>(new DesignOfExperimentImplementation(name, physicalModel, lowerBounds, upperBounds, nbValues))
{
}


DesignOfExperiment::DesignOfExperiment(const String & name, const PhysicalModel & physicalModel,
                                       const String & fileName, OT::Indices columns)
  : TypedInterfaceObject<DesignOfExperimentImplementation>(new DesignOfExperimentImplementation(name, physicalModel, fileName, columns))
{
}

// TODO
// DesignOfExperiment::DesignOfExperiment(const String & name, const PhysicalModel & physicalModel,
//                                        const Experiment & experiment)
//   : TypedInterfaceObject<DesignOfExperimentImplementation>(new DesignOfExperimentImplementation(name, physicalModel, experiment))
// {
// }


/* Default constructor */
DesignOfExperiment::DesignOfExperiment(const DesignOfExperimentImplementation & implementation)
  : TypedInterfaceObject<DesignOfExperimentImplementation>(implementation.clone())
{
}


/* Constructor from implementation */
DesignOfExperiment::DesignOfExperiment(const Implementation & p_implementation)
  : TypedInterfaceObject<DesignOfExperimentImplementation>(p_implementation)
{
  // Initialize any other class members here
  // At last, allocate memory space if needed, but go to destructor to free it
}

/* Constructor from implementation pointer */
DesignOfExperiment::DesignOfExperiment(DesignOfExperimentImplementation * p_implementation)
  : TypedInterfaceObject<DesignOfExperimentImplementation>(p_implementation)
{
  // Initialize any other class members here
  // At last, allocate memory space if needed, but go to destructor to free it
}


void DesignOfExperiment::addObserver(Observer * observer)
{
  getImplementation()->addObserver(observer);
}


PhysicalModel DesignOfExperiment::getPhysicalModel() const
{
  return getImplementation()->getPhysicalModel();
}


void DesignOfExperiment::setPhysicalModel(const PhysicalModel & physicalModel)
{
  getImplementation()->setPhysicalModel(physicalModel);
}


DesignOfExperimentImplementation::Type DesignOfExperiment::getTypeDesignOfExperiment() const
{
  return getImplementation()->getTypeDesignOfExperiment();
}


int DesignOfExperiment::getNumberOfExperiments() const
{
  return getImplementation()->getNumberOfExperiments();
}


NumericalPoint DesignOfExperiment::getValues() const
{
  return getImplementation()->getValues();
}


void DesignOfExperiment::setValues(const NumericalPoint & values)
{
  getImplementation()->setValues(values);
}


NumericalPoint DesignOfExperiment::getLowerBounds() const
{
  return getImplementation()->getLowerBounds();
}


void DesignOfExperiment::setLowerBounds(const NumericalPoint & lowerBounds)
{
  getImplementation()->setLowerBounds(lowerBounds);
}


NumericalPoint DesignOfExperiment::getUpperBounds() const
{
  return getImplementation()->getUpperBounds();
}


void DesignOfExperiment::setUpperBounds(const NumericalPoint & upperBounds)
{
  getImplementation()->setUpperBounds(upperBounds);
}


Indices DesignOfExperiment::getLevels() const
{
  return getImplementation()->getLevels();
}


void DesignOfExperiment::setLevels(const Indices & nbValues)
{
  getImplementation()->setLevels(nbValues);
}


NumericalPoint DesignOfExperiment::getDeltas() const
{
  return getImplementation()->getDeltas();
}


void DesignOfExperiment::setDeltas(const NumericalPoint & deltas)
{
  getImplementation()->setDeltas(deltas);
}


String DesignOfExperiment::getFileName() const
{
  return getImplementation()->getFileName();
}


void DesignOfExperiment::setFileName(const String & fileName)
{
  getImplementation()->setFileName(fileName);
}


Indices DesignOfExperiment::getColumns() const
{
  return getImplementation()->getColumns();
}


void DesignOfExperiment::setColumns(Indices columns)
{
  getImplementation()->setColumns(columns);
}

// TODO
// Experiment DesignOfExperiment::getExperiment() const
// {
//   return getImplementation()->getExperiment();
// }
// 
// 
// void DesignOfExperiment::setExperiment(const Experiment & experiment)
// {
//   getImplementation()->setExperiment(experiment);
// }


Description DesignOfExperiment::getVariableInputsNames() const
{
  return getImplementation()->getVariableInputsNames();
}


NumericalSample DesignOfExperiment::getInputSample()
{
  return getImplementation()->getInputSample();
}


void DesignOfExperiment::setInputSample(const NumericalSample & sample)
{
  getImplementation()->setInputSample(sample);
}


SimulationAnalysisResult DesignOfExperiment::getResult() const
{
  return getImplementation()->getResult();
}


void DesignOfExperiment::clearResult()
{
  getImplementation()->clearResult();
}


void DesignOfExperiment::updateParameters()
{
  getImplementation()->updateParameters();
}


void DesignOfExperiment::eval()
{
  getImplementation()->eval();
}


String DesignOfExperiment::dump() const
{
  return getImplementation()->dump();
}
}