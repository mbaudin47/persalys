// PhysicalModelItem.hxx

#ifndef PHYSICALMODELITEM_H
#define PHYSICALMODELITEM_H

#include "otgui/ObserverItem.hxx"
#include "otgui/InputTableModel.hxx"
#include "otgui/OutputTableModel.hxx"
#include "otgui/PhysicalModel.hxx"

namespace OTGUI {
class PhysicalModelItem : public ObserverItem
{
  Q_OBJECT

public:
  PhysicalModelItem(const PhysicalModel & physicalModel);

  virtual ~PhysicalModelItem();

  PhysicalModel getPhysicalModel() const;
  void setPhysicalModel(const PhysicalModel & physicalModel);

  void setData(const QVariant & value, int role);

  virtual void update(Observable * source, const std::string & message);

public slots:
signals:
  void inputChanged(InputCollection);
  void outputChanged(OutputCollection);

private:
  PhysicalModel physicalModel_;
};
}
#endif