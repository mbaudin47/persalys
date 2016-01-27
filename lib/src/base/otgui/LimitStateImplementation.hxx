// LimitStateImplementation.hxx

#ifndef LIMITSTATEIMPLEMENTATION_H
#define LIMITSTATEIMPLEMENTATION_H

#include "PhysicalModel.hxx"

#include "Event.hxx"

namespace OTGUI {
class LimitStateImplementation : public OT::PersistentObject, public Observable
{
public:
  typedef OT::Pointer<LimitStateImplementation>       Implementation;

  LimitStateImplementation(const std::string & name, const PhysicalModel & physicalModel,
                           const std::string & outputName,
                           const OT::ComparisonOperator & failure, const double & threshold);

  virtual LimitStateImplementation * clone() const;

  PhysicalModel getPhysicalModel() const;
  void setPhysicalModel(const PhysicalModel & physicalModel);

  std::string getOutputName() const;
  void setOutputName(const std::string & outputName);

  OT::ComparisonOperator getOperator() const;
  void setOperator(const OT::ComparisonOperator & comparisonOperator);

  double getThreshold() const;
  void setThreshold(const double & threshold);

  OT::Event getEvent();

  std::string dump() const;

private:
  PhysicalModel physicalModel_;
  std::string outputName_;
  OT::ComparisonOperator operator_;
  double threshold_;
};
}
#endif