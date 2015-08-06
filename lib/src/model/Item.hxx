// Item.hxx

#ifndef ITEM_H
#define ITEM_H

#include <QStandardItem>
#include "Observer.hxx"

namespace OTGUI {
class Item : public QObject, public QStandardItem, public Observer
{
  Q_OBJECT

public:
  Item(std::string name, QString typeName);

  virtual ~Item();

  QVariant data(int role = Qt::EditRole) const;
  void setData(const QVariant& value, int role);

private:
  QVariant type_;
  std::string name_;
};
}
#endif