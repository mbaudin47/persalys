//                                               -*- C++ -*-
/**
 *  @brief QStandardItem, observer of an otStudy
 *
 *  Copyright 2015-2018 EDF-Phimeca
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
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef OTGUI_OTSTUDYITEM_HXX
#define OTGUI_OTSTUDYITEM_HXX

#include "otgui/PhysicalModelDiagramItem.hxx"
#include "otgui/DataModelDiagramItem.hxx"
#include "otgui/OTStudy.hxx"

namespace OTGUI
{
class OTGUI_API OTStudyItem : public OTguiItem, public Observer
{
  Q_OBJECT

public:
  OTStudyItem(const OTStudy & otStudy);

  void update(Observable * source, const OT::String & message);

  void appendItem(DesignOfExperiment & dataModel);
  void appendItem(PhysicalModel & physicalModel);
  void appendItem(LimitState & limitState);
  void appendItem(Analysis & analysis);

  virtual QVariant data(int role) const;
  void setData(const QVariant & value, int role);
  OTStudy getOTStudy() const;

  void exportPythonScript(const QString& fileName);

protected slots:
  virtual void requestRemove();

public slots:
  void updateIcon();
  void createSymbolicModel();
  void createPythonModel();
#ifdef OTGUI_HAVE_YACS
  void createYACSModel();
#endif
#ifdef OTGUI_HAVE_OTFMI
  void createFMIModel();
#endif
  void createDataModel();
  void emitSave();
  void emitSaveAs();
  bool save(const QString&);
  void emitClose();
  void appendMetaModelItem(PhysicalModel metaModel);
signals:
  void statusChanged();
  void exportRequested();
  void saveRequested();
  void saveAsRequested();
  void saveRequested(OTStudyItem* item);
  void saveAsRequested(OTStudyItem* item);
  void closeRequested(OTStudyItem* item);
  void dataModelItemCreated(DataModelDiagramItem*);
  void physicalModelItemCreated(PhysicalModelDiagramItem*);

protected:
  void buildActions();

private:
  OTStudy otStudy_;
  QAction * newSymbolicModel_;
  QAction * newPythonModel_;
#ifdef OTGUI_HAVE_YACS
  QAction * newYACSModel_;
#endif
#ifdef OTGUI_HAVE_OTFMI
  QAction * newFMIModel_;
#endif
  QAction * newDataModel_;
  QAction * exportAction_;
  QAction * saveAction_;
  QAction * saveAsAction_;
  QAction * closeAction_;
};
}
#endif
