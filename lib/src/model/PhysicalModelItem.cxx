#include "otgui/PhysicalModelItem.hxx"

namespace OTGUI {

PhysicalModelItem::PhysicalModelItem(const PhysicalModel & physicalModel)
  : ObserverItem(physicalModel.getName().c_str(), QString("PhysicalModel"))
  , physicalModel_(physicalModel)
{
}


PhysicalModelItem::~PhysicalModelItem()
{
// delete all children (analysis) + physicalModel_
}


void PhysicalModelItem::setData(const QVariant & value, int role)
{
  switch (role)
  {
    case Qt::EditRole:
      physicalModel_.setName(value.toString().toStdString());
      ObserverItem::setData(value, role);
      break;
  }
}


PhysicalModel PhysicalModelItem::getPhysicalModel() const
{
  return physicalModel_;
}


void PhysicalModelItem::updatePhysicalModel(const PhysicalModel & physicalModel)
{
  physicalModel_ = physicalModel;
  physicalModel_.addObserver(this);
}


void PhysicalModelItem::update(Observable* source, const std::string & message)
{
  PhysicalModelImplementation * physicalModelImpl = static_cast<PhysicalModelImplementation*>(source);
  if (message=="inputChanged")
  {
    emit inputChanged(physicalModelImpl->getInputs());
  }
  else if (message=="outputChanged")
  {
    emit outputChanged(physicalModelImpl->getOutputs());
  }
}



}