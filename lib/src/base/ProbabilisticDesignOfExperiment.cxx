//                                               -*- C++ -*-
/**
 *  @brief Class to define designs of experiments
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
#include "persalys/ProbabilisticDesignOfExperiment.hxx"

#include <openturns/MonteCarloExperiment.hxx>
#include <openturns/LHSExperiment.hxx>
#include <openturns/SobolSequence.hxx>
#include <openturns/LowDiscrepancyExperiment.hxx>
#include <openturns/RandomGenerator.hxx>
#include <openturns/PersistentObjectFactory.hxx>
#include <openturns/SimulatedAnnealingLHS.hxx>
#include <openturns/MonteCarloLHS.hxx>


using namespace OT;

namespace PERSALYS
{

CLASSNAMEINIT(ProbabilisticDesignOfExperiment)

static Factory<ProbabilisticDesignOfExperiment> Factory_ProbabilisticDesignOfExperiment;

Description ProbabilisticDesignOfExperiment::DesignNames_;

Description ProbabilisticDesignOfExperiment::GetDesignNames()
{
  if (DesignNames_.isEmpty())
  {
    DesignNames_ = Description();
    DesignNames_.add("LHS");
    DesignNames_.add("SALHS");
    DesignNames_.add("MCLHS");
    DesignNames_.add("MONTE_CARLO");
    DesignNames_.add("QUASI_MONTE_CARLO");
  }

  return DesignNames_;
}


ProbabilisticDesignOfExperiment::ProbabilisticDesignOfExperiment()
  : DesignOfExperimentEvaluation()
  , designName_("LHS")
  , size_(ResourceMap::GetAsUnsignedInteger("WeightedExperiment-DefaultSize"))
{
  isDeterministicAnalysis_ = false;
}


ProbabilisticDesignOfExperiment::ProbabilisticDesignOfExperiment(const String& name,
    const PhysicalModel& physicalModel,
    const UnsignedInteger size,
    const String& designName, const UnsignedInteger mcLhsSize)
  : DesignOfExperimentEvaluation(name, physicalModel)
  , designName_("")
  , size_(0)
  , mcLhsSize_(mcLhsSize)
{
  isDeterministicAnalysis_ = false;
  setDesignName(designName);
  setSize(size);
}


ProbabilisticDesignOfExperiment* ProbabilisticDesignOfExperiment::clone() const
{
  return new ProbabilisticDesignOfExperiment(*this);
}


String ProbabilisticDesignOfExperiment::getDesignName() const
{
  return designName_;
}


void ProbabilisticDesignOfExperiment::setDesignName(const String& name)
{
  if (!GetDesignNames().contains(name))
    throw InvalidArgumentException(HERE) << "Error: the given design of experiments name=" << name << " is unknown.";

  designName_ = name;

  // clear samples
  originalInputSample_.clear();
  initialize();
}


UnsignedInteger ProbabilisticDesignOfExperiment::getSize() const
{
  return size_;
}

UnsignedInteger ProbabilisticDesignOfExperiment::getMCLHSSize() const
{
  return mcLhsSize_;
}

void ProbabilisticDesignOfExperiment::setSize(const UnsignedInteger size)
{
  if (size == 0)
    throw InvalidArgumentException(HERE) << "Error: the size must be > 0.";

  size_ = size;

  // clear samples
  originalInputSample_.clear();
  initialize();
}


void ProbabilisticDesignOfExperiment::setSeed(const UnsignedInteger seed)
{
  SimulationAnalysis::setSeed(seed);

  // clear samples
  originalInputSample_.clear();
  initialize();
}


Sample ProbabilisticDesignOfExperiment::generateInputSample(const UnsignedInteger /*nbSimu*/) const
{
  Sample sample;

  RandomGenerator::SetSeed(getSeed());

  if (designName_ == "LHS")
    sample = LHSExperiment(getPhysicalModel().getDistribution(), size_).generate();
  else if (designName_ == "SALHS")
    sample = SimulatedAnnealingLHS(LHSExperiment(getPhysicalModel().getDistribution(), size_)).generate();
  else if (designName_ == "MCLHS")
    sample = MonteCarloLHS(LHSExperiment(getPhysicalModel().getDistribution(), size_), mcLhsSize_).generate();
  else if (designName_ == "MONTE_CARLO")
    sample = MonteCarloExperiment(getPhysicalModel().getDistribution(), size_).generate();
  else if (designName_ == "QUASI_MONTE_CARLO")
    sample = LowDiscrepancyExperiment(SobolSequence(getPhysicalModel().getStochasticInputNames().getSize()), getPhysicalModel().getDistribution(), size_).generate();
  else
    throw InvalidArgumentException(HERE) << "Error: generateInputSample design name unknown";

  // if there is at least a deterministic variable
  if (getPhysicalModel().getStochasticInputNames().getSize() < getPhysicalModel().getInputDimension())
  {
    Point inValues(getPhysicalModel().getInputDimension());
    Indices variableInputsIndices;
    for (UnsignedInteger i = 0; i < inValues.getSize(); ++i)
    {
      inValues[i] = getPhysicalModel().getInputs()[i].getValue();
      if (getPhysicalModel().getInputs()[i].isStochastic())
        variableInputsIndices.add(i);
    }

    Sample inputSample(size_, inValues);
    for (UnsignedInteger i = 0; i < size_; ++i)
    {
      for (UnsignedInteger j = 0; j < variableInputsIndices.getSize(); ++j)
      {
        inputSample(i, variableInputsIndices[j]) = sample(i, j);
      }
    }
    inputSample.setDescription(getPhysicalModel().getInputNames());
    return inputSample;
  }
  else
  {
    sample.setDescription(getPhysicalModel().getInputNames());
    return sample;
  }
}


Parameters ProbabilisticDesignOfExperiment::getParameters() const
{
  Parameters param;

  String designName = "LHS";
  if (getDesignName() == "SALHS")
    designName = "Simulated annealing LHS";
  else if (getDesignName() == "MCLHS")
    designName = "Monte Carlo LHS";
  else if (getDesignName() == "MONTE_CARLO")
    designName = "Monte Carlo";
  else if (getDesignName() == "QUASI_MONTE_CARLO")
    designName = "Quasi-Monte Carlo";
  param.add("Design name", designName);
  param.add("Outputs of interest", getInterestVariables().__str__());
  param.add("Sample size", getOriginalInputSample().getSize());
  param.add(SimulationAnalysis::getParameters());

  return param;
}


String ProbabilisticDesignOfExperiment::getPythonScript() const
{
  OSS oss;
  oss << getName() << " = persalys.ProbabilisticDesignOfExperiment('" << getName() << "', " << getPhysicalModel().getName() << ", ";
  oss << getSize() << ", '" << getDesignName() << "')\n";
  oss << getName() << ".setSeed(" << getSeed() << ")\n";

  oss << getName() << ".setBlockSize(" << getBlockSize() << ")\n";
  oss << "interestVariables = " << Parameters::GetOTDescriptionStr(getInterestVariables()) << "\n";
  oss << getName() << ".setInterestVariables(interestVariables)\n";

  return oss;
}


/* String converter */
String ProbabilisticDesignOfExperiment::__repr__() const
{
  OSS oss;
  oss << "class=" << GetClassName()
      << " name=" << getName()
      << " physicalModel=" << getPhysicalModel().getName()
      << " design name=" << getDesignName()
      << " size= " << getSize()
      << " seed= " << getSeed()
      << " blockSize=" << getBlockSize();

  return oss;
}


/* Method save() stores the object through the StorageManager */
void ProbabilisticDesignOfExperiment::save(Advocate& adv) const
{
  DesignOfExperimentEvaluation::save(adv);
  adv.saveAttribute("designName_", designName_);
  adv.saveAttribute("size_", size_);
  adv.saveAttribute("mcLhsSize_", mcLhsSize_);
}


/* Method load() reloads the object from the StorageManager */
void ProbabilisticDesignOfExperiment::load(Advocate& adv)
{
  DesignOfExperimentEvaluation::load(adv);
  adv.loadAttribute("designName_", designName_);
  adv.loadAttribute("size_", size_);
  if(adv.hasAttribute("mcLhsSize_"))
     adv.loadAttribute("mcLhsSize_", mcLhsSize_);
}
}
