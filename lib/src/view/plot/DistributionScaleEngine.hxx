#ifndef DISTRIBUTIONSCALEENGINE_HXX
#define DISTRIBUTIONSCALEENGINE_HXX

#include "qwt_scale_engine.h"

namespace OTGUI {

class DistributionScaleEngine : public QwtLinearScaleEngine
{
public:
  DistributionScaleEngine();

  virtual void autoScale(int maxNumSteps, double & x1, double & x2, double & stepSize) const;

protected:
  QwtInterval aligne(const QwtInterval & interval, const double & stepSize, const int & maxNumSteps) const;
};
}
#endif
