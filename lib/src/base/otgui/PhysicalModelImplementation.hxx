// PhysicalModelImplementation.hxx

#ifndef PHYSICALMODELIMPLEMENTATION_H
#define PHYSICALMODELIMPLEMENTATION_H

#include "Input.hxx"
#include "Output.hxx"
#include "Observable.hxx"

#include "NumericalMathFunction.hxx"
#include "RandomVector.hxx"
#include "ComposedDistribution.hxx"

namespace OTGUI {
class PhysicalModelImplementation : public OT::PersistentObject, public Observable
{
public:
  explicit PhysicalModelImplementation(const std::string & name);
  PhysicalModelImplementation(const std::string & name, const InputCollection & inputs,
                              const OutputCollection & outputs);
  PhysicalModelImplementation(const PhysicalModelImplementation & other);
  PhysicalModelImplementation(const PhysicalModelImplementation * other);

  virtual PhysicalModelImplementation * clone() const;

  std::string getName() const;
  void setName(const std::string & name);

  InputCollection getInputs() const;
  Input & getInputByName(const std::string & inputName);
  virtual void setInputs(const InputCollection & inputs);
  void setInputDescription(const std::string & inputName, const std::string & description);
  void setInputValue(const std::string & inputName, const double & value);
  void setInputDistribution(const std::string & inputName, const OT::Distribution & distribution);
  virtual void addInput(const Input & input);
  virtual void removeInput(const std::string & inputName);
  OT::Description getInputNames();
  bool hasAnInputNamed(const std::string & inputName);
  bool hasStochasticInputs();

  OutputCollection getOutputs() const;
  Output & getOutputByName(const std::string & outputName);
  virtual void setOutputs(const OutputCollection & outputs);
  void setOutputDescription(const std::string & outputName, const std::string & description);
  void setOutputFormula(const std::string & outputName, const std::string & formula);
  virtual void addOutput(const Output & output);
  virtual void removeOutput(const std::string & outputName);
  OT::Description getOutputNames();
  bool hasAnOutputNamed(const std::string & outputName);

  OT::ComposedDistribution getComposedDistribution() const;
  OT::RandomVector getInputRandomVector();
  OT::RandomVector getOutputRandomVector(const OT::Description & outputNames);

  virtual OT::NumericalMathFunction getFunction(const OT::Description & outputNames);
  virtual OT::NumericalMathFunction getFunction();

  virtual std::string dump() const;

private:
  std::string name_;
  InputCollection inputs_;
  OutputCollection outputs_;
  OT::Description inputNames_;
  OT::Description outputNames_;
};
}
#endif