//                                               -*- C++ -*-
/**
 *  @brief SRCAnalysis computes the Standard Regression Coefficients
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
#include "otgui/SRCAnalysis.hxx"

#include "openturns/RandomGenerator.hxx"
#include "openturns/CorrelationAnalysis.hxx"
#include "openturns/PersistentObjectFactory.hxx"

using namespace OT;

namespace OTGUI {

CLASSNAMEINIT(SRCAnalysis);

static Factory<SRCAnalysis> RegisteredFactory;

/* Default constructor */
SRCAnalysis::SRCAnalysis()
  : SimulationAnalysis()
  , simulationsNumber_(10000)
{
}


/* Constructor with parameters */
SRCAnalysis::SRCAnalysis(const String & name, const PhysicalModel & physicalModel, const OT::UnsignedInteger nbSimu)
  : SimulationAnalysis(name, physicalModel)
  , simulationsNumber_(nbSimu)
{
}


/* Virtual constructor */
SRCAnalysis* SRCAnalysis::clone() const
{
  return new SRCAnalysis(*this);
}


void SRCAnalysis::run()
{
  try
  {
    // clear result
    stopRequested_ = false;
    result_ = SRCResult();

    RandomGenerator::SetSeed(getSeed());

    NumericalSample inputSample(generateInputSample(simulationsNumber_));

    // evaluate model
    NumericalSample outputSample(0, getInterestVariables().getSize());
    for (UnsignedInteger i=0; i<simulationsNumber_; ++i)
    {
      if (stopRequested_ && i > 1)
        break;

      outputSample.add(computeOutputSample(inputSample[i]));
    }

    NumericalSample indices(0, inputSample.getDimension());

    const NumericalSample effectiveInputSample(inputSample, 0, outputSample.getSize());
    for (UnsignedInteger i=0; i<getInterestVariables().getSize(); ++i)
    {
      Description outputName(1);
      outputName[0] = getInterestVariables()[i];

      indices.add(CorrelationAnalysis::SRC(effectiveInputSample, outputSample.getMarginal(i)));
    }

    indices.setDescription(inputSample.getDescription());
    // set results
    result_ = SRCResult(indices, getInterestVariables());

    // add warning if the model has not an independent copula
    if (!getPhysicalModel().getComposedDistribution().hasIndependentCopula())
    {
      LOGWARN("The model has not an independent copula, the result of the sensitivity analysis could be false.");
    }

    notify("analysisFinished");
  }
  catch (std::exception & ex)
  {
    errorMessage_ = ex.what();
    notify("analysisBadlyFinished");
  }
}


UnsignedInteger SRCAnalysis::getSimulationsNumber() const
{
  return simulationsNumber_;
}


void SRCAnalysis::setSimulationsNumber(const UnsignedInteger number)
{
  simulationsNumber_ = number;
}


SRCResult SRCAnalysis::getResult() const
{
  return result_;
}


String SRCAnalysis::getPythonScript() const
{
  OSS oss;
  oss << getName() << " = otguibase.SRCAnalysis('" << getName() << "', " << getPhysicalModel().getName();
  oss << ", " << getSimulationsNumber() << ")\n";
  if (getInterestVariables().getSize() < getPhysicalModel().getSelectedOutputsNames().getSize())
  {
    oss << "interestVariables = [";
    for (UnsignedInteger i=0; i<getInterestVariables().getSize(); ++i)
    {
      oss << "'" << getInterestVariables()[i] << "'";
      if (i < getInterestVariables().getSize()-1)
        oss << ", ";
    }
    oss << "]\n";
    oss << getName() << ".setInterestVariables(interestVariables)\n";
  }
  oss << getName() << ".setSeed(" << getSeed() << ")\n";

  return oss;
}


bool SRCAnalysis::analysisLaunched() const
{
  return result_.getIndices().getSize() != 0;
}


/* String converter */
String SRCAnalysis::__repr__() const
{
  OSS oss;
  oss << "class=" << GetClassName()
      << " name=" << getName()
      << " physicalModel=" << getPhysicalModel().getName()
      << " simulationsNumber=" << getSimulationsNumber()
      << " seed=" << getSeed();
  return oss;
}


/* Method save() stores the object through the StorageManager */
void SRCAnalysis::save(Advocate & adv) const
{
  SimulationAnalysis::save(adv);
  adv.saveAttribute("simulationsNumber_", simulationsNumber_);
  adv.saveAttribute("result_", result_);
}


/* Method load() reloads the object from the StorageManager */
void SRCAnalysis::load(Advocate & adv)
{
  SimulationAnalysis::load(adv);
  adv.loadAttribute("simulationsNumber_", simulationsNumber_);
  adv.loadAttribute("result_", result_);
}
}