// ModelEvaluation.hxx

#ifndef MODELEVALUATION_H
#define MODELEVALUATION_H

#include "AnalysisImplementation.hxx"
#include "ModelEvaluationResult.hxx"

namespace OTGUI {
class ModelEvaluation : public AnalysisImplementation
{
  CLASSNAME;

public:
  ModelEvaluation(const std::string & name, const PhysicalModel & physicalModel);
  ModelEvaluation(const std::string & name, const PhysicalModel & physicalModel,
                  const OT::NumericalPoint & inputsValues);
  ModelEvaluation(const ModelEvaluation & other);
  virtual ModelEvaluation * clone() const;

  void updateParameters();

  OT::NumericalPoint getInputsValues() const;
  void setInputValue(const int & index, const double & value);

  ModelEvaluationResult getResult() const;

  virtual void run();
  virtual std::string dump() const;
  virtual bool analysisLaunched() const;

protected:
  void initializeParameters(const InputCollection & inputs);

private:
  OT::Description inputNames_;
  ModelEvaluationResult result_;
  OT::NumericalPoint inputsValues_;
};
}
#endif