#include "otgui/ProbabilisticModelItem.hxx"

using namespace OT;

namespace OTGUI {

ProbabilisticModelItem::ProbabilisticModelItem(const PhysicalModel & physicalModel)
  : ObserverItem(QString("Probabilistic Model"), QString("ProbabilisticModel"))
  , physicalModel_(physicalModel)
{
}


ProbabilisticModelItem::~ProbabilisticModelItem()
{
}


PhysicalModel ProbabilisticModelItem::getPhysicalModel() const
{
  return physicalModel_;
}


void ProbabilisticModelItem::updatePhysicalModel(const PhysicalModel & physicalModel)
{
  physicalModel_ = physicalModel;
  physicalModel_.addObserver(this);
  emit physicalModelChanged(physicalModel_);
}


void ProbabilisticModelItem::update(Observable* source, const String & message)
{
  if (message == "inputDistributionChanged" || message == "modelInputsChanged")
  {
    emit inputChanged();
  }
}
}