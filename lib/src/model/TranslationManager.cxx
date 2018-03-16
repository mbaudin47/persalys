//                                               -*- C++ -*-
/**
 *  @brief QObject to translate words
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
#include "otgui/TranslationManager.hxx"

namespace OTGUI
{

boost::bimap< QString, QString > TranslationManager::DistributionsNames_;
boost::bimap< QString, QString > TranslationManager::CopulasNames_;
boost::bimap< QString, QString > TranslationManager::DistributionsParametersNames_;
boost::bimap< QString, QString > TranslationManager::ParametersNames_;


void TranslationManager::InitializeDistributionsNames()
{
  // list of all the available distributions
  DistributionsNames_.insert(type("Arcsine", tr("Arcsine")));
  DistributionsNames_.insert(type("Beta", tr("Beta")));
  DistributionsNames_.insert(type("ChiSquare", tr("ChiSquare")));
  DistributionsNames_.insert(type("Exponential", tr("Exponential")));
  DistributionsNames_.insert(type("Gamma", tr("Gamma")));
  DistributionsNames_.insert(type("Gumbel", "Gumbel"));
  DistributionsNames_.insert(type("InverseNormal", tr("InverseNormal")));
  DistributionsNames_.insert(type("Laplace", tr("Laplace")));
  DistributionsNames_.insert(type("Logistic", tr("Logistic")));
  DistributionsNames_.insert(type("LogNormal", tr("LogNormal")));
  DistributionsNames_.insert(type("LogUniform", tr("LogUniform")));
  DistributionsNames_.insert(type("Normal", tr("Normal")));
  DistributionsNames_.insert(type("Rayleigh", "Rayleigh"));
  DistributionsNames_.insert(type("Student", "Student"));
  DistributionsNames_.insert(type("Trapezoidal", tr("Trapezoidal")));
  DistributionsNames_.insert(type("Triangular", tr("Triangular")));
  DistributionsNames_.insert(type("Uniform", tr("Uniform")));
  DistributionsNames_.insert(type("Weibull", "Weibull"));
}


void TranslationManager::InitializeCopulasNames()
{
  // list of all the available copulas
  CopulasNames_.insert(type("AliMikhailHaq", "Ali-Mikhail-Haq"));
  CopulasNames_.insert(type("Clayton", "Clayton"));
  CopulasNames_.insert(type("FarlieGumbelMorgenstern", "Farlie-Gumbel-Morgenstern"));
  CopulasNames_.insert(type("Frank", "Frank"));
  CopulasNames_.insert(type("Gumbel", "Gumbel"));
  CopulasNames_.insert(type("Normal", tr("Normal")));
}


void TranslationManager::InitializeDistributionsParametersNames()
{
  DistributionsParametersNames_.insert(type("mean", tr("mean")));
  DistributionsParametersNames_.insert(type("standard deviation", tr("standard deviation")));
  DistributionsParametersNames_.insert(type("mean_0", "μ"));
  DistributionsParametersNames_.insert(type("standard_deviation_0", "σ"));
  DistributionsParametersNames_.insert(type("nu", "ν"));
  DistributionsParametersNames_.insert(type("alpha", "α"));
  DistributionsParametersNames_.insert(type("beta", "β"));
  DistributionsParametersNames_.insert(type("theta", "θ"));
  DistributionsParametersNames_.insert(type("gamma", "γ"));
  DistributionsParametersNames_.insert(type("sigma", "σ"));
  DistributionsParametersNames_.insert(type("lambda", "λ"));
  DistributionsParametersNames_.insert(type("mu", "μ"));
  DistributionsParametersNames_.insert(type("sigmaOverMu", "σ/μ"));
  DistributionsParametersNames_.insert(type("muLog", "μ_Log"));
  DistributionsParametersNames_.insert(type("sigmaLog", "σ_Log"));
  DistributionsParametersNames_.insert(type("aLog", "a_Log"));
  DistributionsParametersNames_.insert(type("bLog", "b_Log"));
  DistributionsParametersNames_.insert(type("lowerBound", tr("lower bound")));
  DistributionsParametersNames_.insert(type("upperBound", tr("upper bound")));
}


void TranslationManager::InitializeParametersNames()
{
  ParametersNames_.insert(type("Seed", tr("Seed")));
  ParametersNames_.insert(type("Algorithm", tr("Algorithm")));
  ParametersNames_.insert(type("Taylor expansion moments", tr("Taylor expansion moments")));
  ParametersNames_.insert(type("Confidence level", tr("Confidence level")));
  ParametersNames_.insert(type("Maximum coefficient of variation", tr("Maximum coefficient of variation")));
  ParametersNames_.insert(type("Maximum elapsed time", tr("Maximum elapsed time")));
  ParametersNames_.insert(type("Maximum calls", tr("Maximum calls")));
  ParametersNames_.insert(type("Block size", tr("Block size")));
  ParametersNames_.insert(type("Design name", tr("Design name")));
  ParametersNames_.insert(type("Monte Carlo", tr("Monte Carlo")));
  ParametersNames_.insert(type("Quasi-Monte Carlo", tr("Quasi-Monte Carlo")));
  ParametersNames_.insert(type("Covariance model", tr("Covariance model")));
  ParametersNames_.insert(type("nu", tr("nu")));
  ParametersNames_.insert(type("p", tr("p")));
  ParametersNames_.insert(type("Parameters optimization", tr("Parameters optimization")));
  ParametersNames_.insert(type("Scale", tr("Scale")));
  ParametersNames_.insert(type("Amplitude", tr("Amplitude")));
  ParametersNames_.insert(type("Trend basis", tr("Trend basis")));
  ParametersNames_.insert(type("Leave-one-out validation", tr("Leave-one-out validation")));
  ParametersNames_.insert(type("Analytical validation", tr("Analytical validation")));
  ParametersNames_.insert(type("Test sample validation", tr("Test sample validation")));
  ParametersNames_.insert(type("KFold validation", tr("KFold validation")));
  ParametersNames_.insert(type("Kriging", tr("Kriging")));
  ParametersNames_.insert(type("yes", tr("yes")));
  ParametersNames_.insert(type("no", tr("no")));
  ParametersNames_.insert(type("Constant", tr("Constant")));
  ParametersNames_.insert(type("Linear", tr("Linear")));
  ParametersNames_.insert(type("Quadratic", tr("Quadratic")));
  ParametersNames_.insert(type("Chaos degree", tr("Chaos degree")));
  ParametersNames_.insert(type("Sparse", tr("Sparse")));
  ParametersNames_.insert(type("Functional chaos", tr("Functional chaos")));
  ParametersNames_.insert(type("Sobol", tr("Sobol")));
  ParametersNames_.insert(type("Sample size", tr("Sample size")));
  ParametersNames_.insert(type("Point", tr("Point")));
  ParametersNames_.insert(type("Outputs of interest", tr("Outputs of interest")));
  ParametersNames_.insert(type("Output of interest", tr("Output of interest")));
  ParametersNames_.insert(type("Standardized Regression Coefficients", tr("Standardized Regression Coefficients")));
  ParametersNames_.insert(type("Optimization algorithm", tr("Optimization algorithm")));
  ParametersNames_.insert(type("Optimization type", tr("Optimization type")));
  ParametersNames_.insert(type("Fixed inputs", tr("Fixed inputs")));
  ParametersNames_.insert(type("Solver", tr("Solver")));
  ParametersNames_.insert(type("Analysis", tr("Analysis")));
  ParametersNames_.insert(type("Starting point", tr("Starting point")));
  ParametersNames_.insert(type("Physical starting point", tr("Physical starting point")));
  ParametersNames_.insert(type("Maximum iterations number", tr("Maximum iterations number")));
  ParametersNames_.insert(type("Maximum absolute error", tr("Maximum absolute error")));
  ParametersNames_.insert(type("Maximum relative error", tr("Maximum relative error")));
  ParametersNames_.insert(type("Maximum residual error", tr("Maximum residual error")));
  ParametersNames_.insert(type("Maximum constraint error", tr("Maximum constraint error")));
  ParametersNames_.insert(type("FORM", tr("FORM")));
  ParametersNames_.insert(type("Design point (standard space)", tr("Design point (standard space)")));
  ParametersNames_.insert(type("FORM - Importance sampling", tr("FORM - Importance sampling")));
  ParametersNames_.insert(type("Importance sampling", tr("Importance sampling")));
  ParametersNames_.insert(type("Method", tr("Method")));
  ParametersNames_.insert(type("Level", tr("Level")));
  ParametersNames_.insert(type("Kolmogorov-Smirnov", tr("Kolmogorov-Smirnov")));
  ParametersNames_.insert(type("Number of folds", tr("Number of folds")));
  ParametersNames_.insert(type("Percentage", tr("Percentage")));
  ParametersNames_.insert(type("Bootstrap size", tr("Bootstrap size")));
  ParametersNames_.insert(type("Bootstrap confidence level", tr("Bootstrap confidence level")));
  ParametersNames_.insert(type("Bounds", tr("Bounds")));
  ParametersNames_.insert(type("Design type", tr("Design type")));
  ParametersNames_.insert(type("Grid", tr("Grid")));
  ParametersNames_.insert(type("File", tr("File")));
  ParametersNames_.insert(type("Imported", tr("Imported")));
  ParametersNames_.insert(type("Analytical", tr("Analytical")));
  ParametersNames_.insert(type("Test sample", tr("Test sample")));
  ParametersNames_.insert(type("K-Fold", tr("K-Fold")));
  ParametersNames_.insert(type("Leave-one-out", tr("Leave-one-out")));
  ParametersNames_.insert(type("Physical model", tr("Physical model")));
  ParametersNames_.insert(type("Deterministic inputs", tr("Deterministic inputs")));
  ParametersNames_.insert(type("Stochastic inputs", tr("Stochastic inputs")));
  ParametersNames_.insert(type("Outputs", tr("Outputs")));
  ParametersNames_.insert(type("Limit state", tr("Limit state")));
  ParametersNames_.insert(type("Name", tr("Name")));
  ParametersNames_.insert(type("Description", tr("Description")));
  ParametersNames_.insert(type("Value", tr("Value")));
  ParametersNames_.insert(type("Distribution", tr("Distribution")));
  ParametersNames_.insert(type("Dependence", tr("Dependence")));
  ParametersNames_.insert(type("Group of variables", tr("Group of variables")));
  ParametersNames_.insert(type("Copula", tr("Copula")));
  ParametersNames_.insert(type("Formula", tr("Formula")));
  ParametersNames_.insert(type("File name", tr("File name")));
  ParametersNames_.insert(type("Number of trajectories", tr("Number of trajectories")));
  ParametersNames_.insert(type("Columns", tr("Columns")));
}


QString TranslationManager::GetTranslatedDistributionName(const std::string& name)
{
  if (DistributionsNames_.empty())
    InitializeDistributionsNames();

  bimap_type::left_const_iterator left_iter = DistributionsNames_.left.find(name.c_str());

  if (left_iter != DistributionsNames_.left.end())
    return left_iter->second;

  return name.c_str();
}


QString TranslationManager::GetTranslatedCopulaName(const std::string& name)
{
  if (CopulasNames_.empty())
    InitializeCopulasNames();

  bimap_type::left_const_iterator left_iter = CopulasNames_.left.find(name.c_str());

  if (left_iter != CopulasNames_.left.end())
    return left_iter->second;

  return name.c_str();
}


QString TranslationManager::GetTranslatedDistributionParameterName(const std::string& name)
{
  if (DistributionsParametersNames_.empty())
    InitializeDistributionsParametersNames();

  QString paramName = name.c_str();
  if (paramName == "mean")
    paramName = "mu";
  else if (paramName == "standard deviation")
    paramName = "sigma";

  bimap_type::left_const_iterator left_iter = DistributionsParametersNames_.left.find(paramName);

  if (left_iter != DistributionsParametersNames_.left.end())
    return left_iter->second;

  return paramName;
}


QString TranslationManager::GetTranslatedParameterName(const std::string& name)
{
  if (ParametersNames_.empty())
    InitializeParametersNames();

  bimap_type::left_const_iterator left_iter = ParametersNames_.left.find(name.c_str());

  if (left_iter != ParametersNames_.left.end())
    return left_iter->second;

  return name.c_str();
}


std::string TranslationManager::GetDistributionName(const QString& name)
{
  if (DistributionsNames_.empty())
    InitializeDistributionsNames();

  bimap_type::right_const_iterator right_iter = DistributionsNames_.right.find(name);

  if (right_iter != DistributionsNames_.right.end())
    return right_iter->second.toStdString();

  return name.toStdString();
}


std::string TranslationManager::GetCopulaName(const QString& name)
{
  if (CopulasNames_.empty())
    InitializeCopulasNames();

  bimap_type::right_const_iterator right_iter = CopulasNames_.right.find(name);

  if (right_iter != CopulasNames_.right.end())
    return right_iter->second.toStdString();

  return name.toStdString();
}


/* Get the list of the available distributions */
QStringList TranslationManager::GetAvailableDistributions()
{
  if (DistributionsNames_.empty())
    InitializeDistributionsNames();

  QStringList distributions;
  for (bimap_type::left_const_iterator left_iter = DistributionsNames_.left.begin(), iend = DistributionsNames_.left.end(); left_iter != iend; ++left_iter)
    distributions << left_iter->second;
  return distributions;
}


/* Get the list of the available copulas */
QStringList TranslationManager::GetAvailableCopulas()
{
  if (CopulasNames_.empty())
    InitializeCopulasNames();

  QStringList copulas;
  for (bimap_type::left_const_iterator left_iter = CopulasNames_.left.begin(), iend = CopulasNames_.left.end(); left_iter != iend; ++left_iter)
    copulas << left_iter->second;
  return copulas;
}
}
