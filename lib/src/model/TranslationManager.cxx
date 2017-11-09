//                                               -*- C++ -*-
/**
 *  @brief QObject to translate words
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
#include "otgui/TranslationManager.hxx"

namespace OTGUI
{

boost::bimap< QString, QString > TranslationManager::DistributionsNames_;
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
  DistributionsNames_.insert(type("Gumbel", tr("Gumbel")));
  DistributionsNames_.insert(type("InverseNormal", tr("InverseNormal")));
  DistributionsNames_.insert(type("Laplace", tr("Laplace")));
  DistributionsNames_.insert(type("Logistic", tr("Logistic")));
  DistributionsNames_.insert(type("LogNormal", tr("LogNormal")));
  DistributionsNames_.insert(type("LogUniform", tr("LogUniform")));
  DistributionsNames_.insert(type("Normal", tr("Normal")));
  DistributionsNames_.insert(type("Rayleigh", tr("Rayleigh")));
  DistributionsNames_.insert(type("Student", tr("Student")));
  DistributionsNames_.insert(type("Trapezoidal", tr("Trapezoidal")));
  DistributionsNames_.insert(type("Triangular", tr("Triangular")));
  DistributionsNames_.insert(type("Uniform", tr("Uniform")));
  DistributionsNames_.insert(type("Weibull", tr("Weibull")));
}

void TranslationManager::InitializeDistributionsParametersNames()
{
  DistributionsParametersNames_.insert(type("mean", tr("mean")));
  DistributionsParametersNames_.insert(type("standard deviation", tr("standard deviation")));
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
  ParametersNames_.insert(type("Output of interest", tr("Outputs of interest")));
  ParametersNames_.insert(type("Standardized Regression Coefficients", tr("Standardized Regression Coefficients")));
  ParametersNames_.insert(type("Optimization algorithm", tr("Optimization algorithm")));
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


QString TranslationManager::GetTranslatedDistributionParameterName(const std::string& name)
{
  if (DistributionsParametersNames_.empty())
    InitializeDistributionsParametersNames();

  bimap_type::left_const_iterator left_iter = DistributionsParametersNames_.left.find(name.c_str());

  if (left_iter != DistributionsParametersNames_.left.end())
    return left_iter->second;

  return name.c_str();
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
}
