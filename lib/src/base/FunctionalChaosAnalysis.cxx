//                                               -*- C++ -*-
/**
 *  @brief Class to define functional chaos
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
#include "otgui/FunctionalChaosAnalysis.hxx"

#include "otgui/MetaModel.hxx"

#include <openturns/LinearEnumerateFunction.hxx>
#include <openturns/FixedStrategy.hxx>
#include <openturns/LeastSquaresStrategy.hxx>
#include <openturns/BasisSequenceFactory.hxx>
#include <openturns/LeastSquaresMetaModelSelectionFactory.hxx>
#include <openturns/LegendreFactory.hxx>
#include <openturns/LARS.hxx>
#include <openturns/Uniform.hxx>
#include <openturns/StandardDistributionPolynomialFactory.hxx>
#include <openturns/FunctionalChaosRandomVector.hxx>
#include <openturns/PersistentObjectFactory.hxx>
#include <openturns/SpecFunc.hxx>

using namespace OT;

namespace OTGUI
{

CLASSNAMEINIT(FunctionalChaosAnalysis)

static Factory<FunctionalChaosAnalysis> Factory_FunctionalChaosAnalysis;

/* Default constructor */
FunctionalChaosAnalysis::FunctionalChaosAnalysis()
  : MetaModelAnalysis()
  , polynomialFamilyCollection_()
  , chaosDegree_(2)
  , sparseChaos_(false)
  , result_()
{
}


/* Constructor with parameters */
FunctionalChaosAnalysis::FunctionalChaosAnalysis(const String& name, const DesignOfExperiment& designOfExperiment)
  : MetaModelAnalysis(name, designOfExperiment)
  , polynomialFamilyCollection_()
  , chaosDegree_(2)
  , sparseChaos_(false)
  , result_()
{
}


/* Constructor with parameters */
FunctionalChaosAnalysis::FunctionalChaosAnalysis(const String& name, const Analysis& analysis)
  : MetaModelAnalysis(name, analysis)
  , polynomialFamilyCollection_()
  , chaosDegree_(2)
  , sparseChaos_(false)
  , result_()
{
}


/* Virtual constructor */
FunctionalChaosAnalysis* FunctionalChaosAnalysis::clone() const
{
  return new FunctionalChaosAnalysis(*this);
}


UnsignedInteger FunctionalChaosAnalysis::getChaosDegree() const
{
  return chaosDegree_;
}


void FunctionalChaosAnalysis::setChaosDegree(const UnsignedInteger degree)
{
  chaosDegree_ = degree;
}


bool FunctionalChaosAnalysis::getSparseChaos() const
{
  return sparseChaos_;
}


void FunctionalChaosAnalysis::setSparseChaos(const bool sparse)
{
  sparseChaos_ = sparse;
}


OrthogonalProductPolynomialFactory::PolynomialFamilyCollection FunctionalChaosAnalysis::getPolynomialFamilyCollection()
{
  if (polynomialFamilyCollection_.isEmpty())
  {
    // distribution:
    ComposedDistribution distribution(getDistribution());

    // adaptiveStrategy
    for (UnsignedInteger i = 0; i < distribution.getDimension(); ++i)
    {
      const Distribution marginal(distribution.getMarginal(i));
      const StandardDistributionPolynomialFactory factory(marginal);
      polynomialFamilyCollection_.add(factory);
    }
  }
  return polynomialFamilyCollection_;
}


void FunctionalChaosAnalysis::run()
{
  isRunning_ = true;
  try
  {
    // clear result
    initialize();
    result_ = FunctionalChaosAnalysisResult();

    // check
    if (designOfExperiment_.getInputSample().getSize() * designOfExperiment_.getOutputSample().getSize() == 0)
      throw InvalidArgumentException(HERE) << "The design of experiments must contains not empty input AND output samples";
    if (designOfExperiment_.getInputSample().getSize() != designOfExperiment_.getOutputSample().getSize())
      throw InvalidArgumentException(HERE) << "The input sample and the output sample must have the same size";
    if (testSampleValidation() && (designOfExperiment_.getInputSample().getSize() * getTestSampleValidationPercentageOfPoints() / 100 < 3))
      throw InvalidArgumentException(HERE) << "Test sample validation: The test sample must contain at least three points. Here size * k / 100 = " << (designOfExperiment_.getInputSample().getSize() * getTestSampleValidationPercentageOfPoints() / 100);
    if (kFoldValidation() && (designOfExperiment_.getInputSample().getSize() / getKFoldValidationNumberOfFolds() < 3))
      throw InvalidArgumentException(HERE) << "K-Fold validation: each fold must contain at least three points. Here size / k = " << (designOfExperiment_.getInputSample().getSize() / getKFoldValidationNumberOfFolds());


    // get effective samples
    const Sample effectiveInputSample(getEffectiveInputSample());
    const Sample effectiveOutputSample(getEffectiveOutputSample());

    // check chaos degree
    if (!getSparseChaos())
    {
      const UnsignedInteger inputDimension = effectiveInputSample.getDimension();
      const UnsignedInteger size = designOfExperiment_.getOutputSample().getSize();
      const UnsignedInteger minimumSize  = SpecFunc::BinomialCoefficient(chaosDegree_ + inputDimension, chaosDegree_);
      if (size < minimumSize)
        throw InvalidArgumentException(HERE) << "Design of experiments size too small : "
                                             << size
                                             << ". It must be superior or equal to C(degree+nbInputs, degree) = "
                                             << minimumSize << ")\n";
    }

    // create FunctionalChaosAlgorithm and run it
    FunctionalChaosAlgorithm functionalChaos(buildFunctionalChaosAlgorithm(effectiveInputSample, effectiveOutputSample));
    functionalChaos.run();

    // set result_
    result_.outputSample_ = effectiveOutputSample;
    result_.functionalChaosResult_ = functionalChaos.getResult();

    // build metamodel
    Function metamodelFunction(result_.functionalChaosResult_.getMetaModel());
    Description variablesNames(effectiveInputSample.getDescription());
    variablesNames.add(effectiveOutputSample.getDescription());
    metamodelFunction.setDescription(variablesNames);
    buildMetaModel(result_, metamodelFunction);

    result_.metaModelOutputSample_ = metamodelFunction(effectiveInputSample);

    // post process
    try
    {
      postProcessFunctionalChaosResult(effectiveInputSample);
    }
    catch (std::exception & ex)
    {
      errorMessage_ = OSS() << "Impossible to compute Sobol indices and moments.\n" << ex.what() << "\nTry to increase the size of the design of experiments.";
    }

    // validation
    validateMetaModelResult(result_, effectiveInputSample);

    notify("analysisFinished");
  }
  catch (std::exception & ex)
  {
    errorMessage_ =  OSS() << errorMessage_ << ex.what();
    notify("analysisBadlyFinished");
  }
  isRunning_ = false;
}


Function FunctionalChaosAnalysis::runAlgo(const Sample& inputSample, const Sample& outputSample)
{
  FunctionalChaosAlgorithm functionalChaos(buildFunctionalChaosAlgorithm(inputSample, outputSample));
  functionalChaos.run();

  return functionalChaos.getResult().getMetaModel();
}


FunctionalChaosAlgorithm FunctionalChaosAnalysis::buildFunctionalChaosAlgorithm(const Sample& inputSample, const Sample& outputSample)
{
  const UnsignedInteger inputDimension = inputSample.getDimension();
  const EnumerateFunction phi(inputDimension);

  // adaptiveStrategy
  const OrthogonalProductPolynomialFactory multivariateBasis(getPolynomialFamilyCollection(), phi);
  const UnsignedInteger baseDim = LinearEnumerateFunction(inputDimension).getStrataCumulatedCardinal(chaosDegree_);
  const FixedStrategy adaptiveStrategy(multivariateBasis, baseDim);

  // projectionStrategy
  LeastSquaresStrategy projectionStrategy;
  if (sparseChaos_)
  {
    BasisSequenceFactory basisSequenceFactory = LARS();
    basisSequenceFactory.setMaximumRelativeConvergence(-1.0);
    projectionStrategy = LeastSquaresStrategy(LeastSquaresMetaModelSelectionFactory(basisSequenceFactory,
                                                               CorrectedLeaveOneOut()));
  }

  // FunctionalChaosAlgorithm
  FunctionalChaosAlgorithm functionalChaos(inputSample,
      outputSample,
      getDistribution(),
      adaptiveStrategy,
      projectionStrategy);

  return functionalChaos;
}


void FunctionalChaosAnalysis::postProcessFunctionalChaosResult(const Sample& inputSample)
{
  // check
  if (!result_.outputSample_.getSize())
    throw InvalidValueException(HERE) << "Problem during the creation of the metamodel: The outputSample is empty.\n";
  if (!result_.functionalChaosResult_.getCoefficients().getSize())
    throw InvalidValueException(HERE) << "Problem during the creation of the metamodel: The FunctionalChaosResult is empty.\n";

  // get dim
  const UnsignedInteger inputDimension = inputSample.getDimension();
  const UnsignedInteger outputDimension = result_.outputSample_.getDimension();

  // get FunctionalChaosRandomVector
  const FunctionalChaosRandomVector vector(result_.functionalChaosResult_);

  // mean
  result_.mean_ = vector.getMean();

  // variance - sobol indices
  Point variance(outputDimension);
  Sample firstOrderIndices(outputDimension, inputDimension);
  firstOrderIndices.setDescription(inputSample.getDescription());
  Sample totalIndices(outputDimension, inputDimension);

  for (UnsignedInteger i = 0; i < outputDimension; ++i)
  {
    variance[i] = vector.getCovariance()(i, i);

    for (UnsignedInteger j = 0; j < inputDimension; ++j)
    {
      firstOrderIndices[i][j] = vector.getSobolIndex(j, i);
      totalIndices[i][j] = vector.getSobolTotalIndex(j, i);
    }
  }
  result_.variance_ = variance;
  result_.sobolResult_ = SobolResult(firstOrderIndices, totalIndices, result_.outputSample_.getDescription());
}


void FunctionalChaosAnalysis::computeAnalyticalValidation(MetaModelAnalysisResult& result, const Sample& inputSample)
{
  if (stopRequested_)
  {
    analyticalValidation_ = false;
    return;
  }
  informationMessage_ = "The analytical validation is running.";
  notify("informationMessageUpdated");

  // retrieve chaos result
  FunctionalChaosAnalysisResult chaosResult(*dynamic_cast<FunctionalChaosAnalysisResult*>(&result));

  // get metamodel result
  Function metamodel(chaosResult.getFunctionalChaosResult().getMetaModel());
  // get polynom basis
  Basis reducedBasis(chaosResult.getFunctionalChaosResult().getReducedBasis());
  // get marginals transformation
  const Function transformation(chaosResult.getFunctionalChaosResult().getTransformation());

  // compute basis matrix at the points of inputSample
  const UnsignedInteger basisDim = reducedBasis.getSize();
  Matrix A(inputSample.getSize(), basisDim);
  for (UnsignedInteger i = 0; i < basisDim; ++i)
  {
    Sample outSample_i(reducedBasis[i](transformation(inputSample)));
    for (UnsignedInteger j = 0; j < inputSample.getSize(); ++j)
    {
      A(j, i) = outSample_i(j, 0);
    }
  }

  // (A^t.A)
  CovarianceMatrix AtA(A.computeGram(true));

  // (A^t.A)^{-1}
  const Matrix AtA_inv(AtA.solveLinearSystem(IdentityMatrix(basisDim)).getImplementation());

  // A.(A^t.A)^{-1}.A^t
  const Matrix H(A * AtA_inv * A.transpose());

  Point Hdiag(H.getNbColumns());
  for (UnsignedInteger i = 0; i < H.getNbColumns(); ++i)
    Hdiag[i] = H(i, i);

  // compute Q2
  Point q2(result.outputSample_.getDimension());
  const Point variance(result.outputSample_.computeVariance());

  for (UnsignedInteger i = 0; i < result.outputSample_.getDimension(); ++i)
  {
    // sum[ ((ŷ_j - y_j) / (1 - h_j))^2 ]
    double quadraticResidual = 0.;
    for (UnsignedInteger j = 0; j < result.outputSample_.getSize(); ++j)
    {
      const double diff = (result.metaModelOutputSample_[j][i] - result.outputSample_[j][i]) / (1 - Hdiag[j]);
      quadraticResidual += diff * diff;
    }
    // 1 - sum[ ((ŷ_j - y_j) / (1 - h_j))^2 ] / (n-1) / Var Y
    q2[i] = 1.0 - (quadraticResidual / (result.outputSample_.getSize() - 1)) / variance[i];
  }
  result.analyticalValidation_.q2_ = q2;
}


FunctionalChaosAnalysisResult FunctionalChaosAnalysis::getResult() const
{
  return result_;
}


Parameters FunctionalChaosAnalysis::getParameters() const
{
  Parameters param;

  param.add("Algorithm", "Functional chaos");
  param.add("Outputs of interest", getInterestVariables().__str__());

  param.add("Chaos degree", getChaosDegree());
  param.add("Sparse", getSparseChaos() ? "yes" : "no");

  param.add("Analytical validation", analyticalValidation() ? "yes" : "no");
  param.add("Test sample validation", testSampleValidation() ? "yes" : "no");
  if (testSampleValidation())
  {
    param.add("Percentage", getTestSampleValidationPercentageOfPoints());
    param.add("Seed", getTestSampleValidationSeed());
  }
  param.add("KFold validation", kFoldValidation() ? "yes" : "no");
  if (kFoldValidation())
  {
    param.add("Number of folds", getKFoldValidationNumberOfFolds());
    param.add("Seed", getKFoldValidationSeed());
  }
  param.add("Leave-one-out validation", leaveOneOutValidation() ? "yes" : "no");

  return param;
}


String FunctionalChaosAnalysis::getPythonScript() const
{
  OSS oss;
  oss << getName() << " = otguibase.FunctionalChaosAnalysis('" << getName() << "', " << getDesignOfExperiment().getName() << ")\n";
  if (getInterestVariables().getSize() < getDesignOfExperiment().getOutputSample().getDimension())
  {
    oss << "interestVariables = " << Parameters::GetOTDescriptionStr(getInterestVariables()) << "\n";
    oss << getName() << ".setInterestVariables(interestVariables)\n";
  }
  oss << getName() << ".setChaosDegree(" << getChaosDegree() << ")\n";
  oss << getName() << ".setSparseChaos(" << (getSparseChaos() ? "True" : "False") << ")\n";

  // validation
  oss << getName() << ".setAnalyticalValidation(" << (analyticalValidation() ? "True" : "False") << ")\n";
  oss << getName() << ".setTestSampleValidation(" << (testSampleValidation() ? "True" : "False") << ")\n";
  if (testSampleValidation())
  {
    oss << getName() << ".setTestSampleValidationPercentageOfPoints(" << getTestSampleValidationPercentageOfPoints() << ")\n";
    oss << getName() << ".setTestSampleValidationSeed(" << getTestSampleValidationSeed() << ")\n";
  }
  oss << getName() << ".setKFoldValidation(" << (kFoldValidation() ? "True" : "False") << ")\n";
  if (kFoldValidation())
  {
    oss << getName() << ".setKFoldValidationNumberOfFolds(" << getKFoldValidationNumberOfFolds() << ")\n";
    oss << getName() << ".setKFoldValidationSeed(" << getKFoldValidationSeed() << ")\n";
  }
  oss << getName() << ".setLeaveOneOutValidation(" << (leaveOneOutValidation() ? "True" : "False") << ")\n";

  return oss;
}


bool FunctionalChaosAnalysis::analysisLaunched() const
{
  return getResult().getMetaModelOutputSample().getSize();
}


/* String converter */
String FunctionalChaosAnalysis::__repr__() const
{
  OSS oss;
  oss << MetaModelAnalysis::__repr__()
      << " chaosDegree=" << getChaosDegree()
      << " sparseChaos=" << getSparseChaos();
  return oss;
}


/* Method save() stores the object through the StorageManager */
void FunctionalChaosAnalysis::save(Advocate& adv) const
{
  MetaModelAnalysis::save(adv);
  adv.saveAttribute("chaosDegree_", chaosDegree_);
  adv.saveAttribute("sparseChaos_", sparseChaos_);
  adv.saveAttribute("result_", result_);
}


/* Method load() reloads the object from the StorageManager */
void FunctionalChaosAnalysis::load(Advocate& adv)
{
  MetaModelAnalysis::load(adv);
  adv.loadAttribute("chaosDegree_", chaosDegree_);
  adv.loadAttribute("sparseChaos_", sparseChaos_);
  adv.loadAttribute("result_", result_);
}


}
