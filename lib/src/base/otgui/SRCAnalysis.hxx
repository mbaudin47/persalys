// SRCAnalysis.hxx

#ifndef SRCANALYSIS_H
#define SRCANALYSIS_H

#include "SimulationAnalysis.hxx"
#include "SRCResult.hxx"

namespace OTGUI {
class SRCAnalysis : public SimulationAnalysis
{
  CLASSNAME;

public:
  SRCAnalysis(const OT::String & name, const PhysicalModel & physicalModel, const OT::UnsignedInteger nbSimu=10000);

  virtual SRCAnalysis * clone() const;

  SRCResult getResult() const;

  virtual void run();
  virtual OT::String dump() const;
  virtual bool analysisLaunched() const;

private:
  SRCResult result_;
};
}
#endif