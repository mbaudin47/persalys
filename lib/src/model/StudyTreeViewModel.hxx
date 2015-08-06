// StudyTreeViewModel.hxx


#ifndef STUDYTREEVIEWMODEL_H
#define STUDYTREEVIEWMODEL_H

#include "StudyItem.hxx"
#include "PhysicalModelItem.hxx"
#include "ParametricCalculusItem.hxx"

#include <QStandardItemModel>
#include <QStandardItem>

#include <string>

namespace OTGUI {
class StudyTreeViewModel : public QStandardItemModel, public Observer
{
  Q_OBJECT

public:
  StudyTreeViewModel();

  virtual ~StudyTreeViewModel();

  void createNewStudy();
  void addStudyItem(OTStudy * study);

  void addPhysicalModelItem(QModelIndex parentIndex);

  ParametricCalculusItem * addParametricCalculusItem(QModelIndex parentIndex);

  virtual void update(Observable * source, std::string message);

public slots:
signals:
  void newPhysicalModelCreated(PhysicalModelItem*);
  void newParametricCalculusCreated(ParametricCalculusItem*);

private:
  QStandardItem * rootNode_;
};
}
#endif