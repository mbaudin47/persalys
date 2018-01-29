//                                               -*- C++ -*-
/**
 *  @brief Abstract top-level class for all physical models
 *
 *  Copyright 2015-2017 EDF-Phimeca
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
#ifndef OTGUI_PHYSICALMODELIMPLEMENTATION_HXX
#define OTGUI_PHYSICALMODELIMPLEMENTATION_HXX

#include "Input.hxx"
#include "Output.hxx"
#include "Observable.hxx"
#include "LaunchParametersVisitor.hxx"

#include <openturns/Function.hxx>
#include <openturns/RandomVector.hxx>
#include <openturns/ComposedDistribution.hxx>
#include <openturns/ComposedCopula.hxx>

namespace OTGUI
{
class OTGUI_API PhysicalModelImplementation : public OT::PersistentObject, public Observable
{
public:
  /** Constructor with parameters */
  explicit PhysicalModelImplementation(const OT::String & name);

  /** Constructor with parameters */
  PhysicalModelImplementation(const OT::String & name,
                              const InputCollection & inputs,
                              const OutputCollection & outputs);

  /** Virtual constructor */
  virtual PhysicalModelImplementation * clone() const;

  /** Comparison operators */
  OT::Bool operator ==(const PhysicalModelImplementation & other) const;
  OT::Bool operator !=(const PhysicalModelImplementation & other) const;

  OT::UnsignedInteger getInputDimension() const;
  InputCollection getInputs() const;
  Input & getInputByName(const OT::String & inputName);
  Input getInputByName(const OT::String & inputName) const;
  virtual void setInputs(const InputCollection & inputs);
  void setInputName(const OT::String & inputName, const OT::String & newName);
  void setInputDescription(const OT::String & inputName, const OT::String & description);
  void setInputValue(const OT::String & inputName, const double & value);
  void setDistribution(const OT::String & inputName, const OT::Distribution & distribution);
  void setDistributionParametersType(const OT::String & inputName, const OT::UnsignedInteger & distributionParametersType);
  void setFiniteDifferenceStep(const OT::String& inputName, const double& step);
  virtual void addInput(const Input & input);
  virtual void removeInput(const OT::String & inputName);
  void clearInputs();
  OT::Description getInputNames() const;
  OT::Description getStochasticInputNames() const;
  bool hasInputNamed(const OT::String & inputName) const;
  bool hasStochasticInputs() const;

  OT::UnsignedInteger getOutputDimension() const;
  OutputCollection getOutputs() const;
  Output & getOutputByName(const OT::String & outputName);
  Output getOutputByName(const OT::String & outputName) const;
  virtual void setOutputs(const OutputCollection & outputs);
  virtual void setOutputName(const OT::String & outputName, const OT::String & newName);
  void setOutputDescription(const OT::String & outputName, const OT::String & description);
  void setOutputValue(const OT::String & outputName, const double & value);
  void selectOutput(const OT::String & outputName, const bool selected);
  virtual void addOutput(const Output & output);
  virtual void removeOutput(const OT::String & outputName);
  void clearOutputs();
  OT::Description getOutputNames() const;
  bool hasOutputNamed(const OT::String & outputName) const;
  OT::Description getSelectedOutputsNames() const;

  OT::Distribution getDistribution() const;
  OT::RandomVector getInputRandomVector() const;
  OT::RandomVector getOutputRandomVector(const OT::Description & outputNames) const;

  OT::Function getFunction() const;
  OT::Function getFunction(const OT::Description& outputNames) const;
  OT::Function getFunction(const OT::String & outputName) const;
  OT::Function getRestrictedFunction() const;
  OT::Function getRestrictedFunction(const OT::Description & outputNames) const;

  OT::Copula getCopula() const;
  OT::Collection<OT::Copula> getCopulaCollection() const;
  void setCopula(const OT::Description& inputNames, const OT::Copula & copula);

  bool isValid() const;

  virtual OT::String getPythonScript() const;

  /** String converter */
  virtual OT::String __repr__() const;

  /** Method save() stores the object through the StorageManager */
  void save(OT::Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(OT::Advocate & adv);

  virtual void acceptLaunchParameters(LaunchParametersVisitor* visitor);

protected:
  virtual OT::Function generateFunction(const OT::Description & outputNames) const;

  void updateCopula();
  OT::String getProbaModelPythonScript() const;
  OT::String getCopulaPythonScript() const;
  OT::Point getFiniteDifferenceSteps() const;
  void updateFiniteDifferenceSteps() const;

private:
  void inputsChanged();

private:
  OT::PersistentCollection<Input> inputs_;
  OT::PersistentCollection<Output> outputs_;
  mutable OT::ComposedCopula composedCopula_;
  mutable OT::Point finiteDifferenceSteps_;
};
}
#endif
