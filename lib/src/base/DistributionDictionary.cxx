// DistributionDictionary.cxx

#include "otgui/DistributionDictionary.hxx"

#include "SpecFunc.hxx"
#include "ArcsineMuSigma.hxx"
#include "BetaMuSigma.hxx"
#include "ChiSquare.hxx"
#include "Exponential.hxx"
#include "InverseNormal.hxx"
#include "GammaMuSigma.hxx"
#include "GumbelMuSigma.hxx"
#include "Laplace.hxx"
#include "LogNormalMuSigma.hxx"
#include "Logistic.hxx"
#include "LogUniform.hxx"
#include "Normal.hxx"
#include "Rayleigh.hxx"
#include "Student.hxx"
#include "Trapezoidal.hxx"
#include "Triangular.hxx"
#include "Uniform.hxx"
#include "WeibullMuSigma.hxx"

using namespace OT;

namespace OTGUI {

/* Get the list of the available distributions */
Description DistributionDictionary::GetAvailableDistributions()
{
  Description listDistributions(18);
  listDistributions[0] = "Arcsine";
  listDistributions[1] = "Beta";
  listDistributions[2] = "ChiSquare";
  listDistributions[3] = "Gamma";
  listDistributions[4] = "Gumbel";
  listDistributions[5] = "Exponential";
  listDistributions[6] = "InverseNormal";
  listDistributions[7] = "Laplace";
  listDistributions[8] = "Logistic";
  listDistributions[9] = "LogNormal";
  listDistributions[10] = "LogUniform";
  listDistributions[11] = "Normal";
  listDistributions[12] = "Rayleigh";
  listDistributions[13] = "Student";
  listDistributions[14] = "Trapezoidal";
  listDistributions[15] = "Triangular";
  listDistributions[16] = "Uniform";
  listDistributions[17] = "Weibull";
  return listDistributions;
}


/* Build a distribution with native parameters from the mean value */
Distribution DistributionDictionary::BuildDistribution(const String & distributionName, const double mu)
{
  const double sigma = ComputeSigmaFromMu(mu);
  try
  {
    if (distributionName == "Arcsine")
    {
      return ArcsineMuSigma(mu, sigma).getDistribution();
    }
    else if (distributionName == "Beta")
    {
      return BetaMuSigma(mu, sigma, mu-3*sigma, mu+3*sigma).getDistribution();
    }
    else if (distributionName == "ChiSquare")
    {
      return ChiSquare(1.); // arbitrary value for nu
    }
    else if (distributionName == "Gamma")
    {
      return GammaMuSigma(mu, sigma, mu-10.0*sigma).getDistribution(); // arbitrary gamma
    }
    else if (distributionName == "Gumbel")
    {
      return GumbelMuSigma(mu, sigma).getDistribution();
    }
    else if (distributionName == "Exponential")
    {
      return Exponential(1.0/sigma, mu-sigma);
    }
    else if (distributionName == "InverseNormal")
    {
      if (mu <= 0)
        return InverseNormal(0.01, 0.1);
      return InverseNormal(mu*mu*mu/(sigma*sigma), mu);
    }
    else if (distributionName == "Laplace")
    {
      return Laplace(sqrt(2.0)/sigma, mu);
    }
    else if (distributionName == "Logistic")
    {
      return Logistic(mu, sigma/SpecFunc::PI_SQRT3);
    }
    else if (distributionName == "LogNormal")
    {
      return LogNormalMuSigma(mu, sigma, mu-10.0*sigma).getDistribution(); // arbitrary gamma
    }
    else if (distributionName == "LogUniform")
    {
      double mu2 = mu;
      double sigma2 = sigma;
      if (mu2 <= 0)
      {
        mu2 = sqrt(3.);
        sigma2 = 1.;
      }
      double deltaSquareRoot = sqrt(mu2*mu2+sigma2*sigma2);
      double muLog = log(mu2*mu2/deltaSquareRoot);
      double sigmaLog = sqrt(2*log(deltaSquareRoot/mu2));

      return LogUniform(muLog-sqrt(3.0)*sigmaLog, muLog+sqrt(3.0)*sigmaLog);
    }
    else if (distributionName == "Normal")
    {
      return Normal(mu, sigma);
    }
    else if (distributionName == "Rayleigh")
    {
      return Rayleigh(sigma/0.6551363775620335530939357, mu-sigma*1.253314137315500251207882/0.6551363775620335530939357);
    }
    else if (distributionName == "Student")
    {
      return Student(3., mu, sigma); // arbitrary value for nu
    }
    else if (distributionName == "Trapezoidal")
    {
      return Trapezoidal(mu-2.0*sigma, mu-sigma, mu+sigma, mu+2.0*sigma);
    }
    else if (distributionName == "Triangular")
    {
      return Triangular(mu-sqrt(6.0)*sigma, mu, mu+sqrt(6.0)*sigma);
    }
    else if (distributionName == "Uniform")
    {
      return Uniform(mu-sigma*sqrt(3.0), mu+sigma*sqrt(3.0));
    }
    else if (distributionName == "Weibull")
    {
      return WeibullMuSigma(mu, sigma, mu-10.0*sigma).getDistribution(); // arbitrary gamma
    }
    else
    {
      throw InvalidArgumentException(HERE) << "The given distribution " << distributionName << "is not available.";
    }
  }
  catch (InvalidArgumentException & ex)
  {
    std::cerr << "Error when creating the distribution " << distributionName << std::endl;
    throw InvalidArgumentException(HERE) << ex.what();
  }
}


/* Compute standard deviation from mean */
double DistributionDictionary::ComputeSigmaFromMu(const double mu)
{
  double sigma = 0.1*fabs(mu);
  if (sigma < 1e-3)
    sigma = 1.0;
  return sigma;
}
}