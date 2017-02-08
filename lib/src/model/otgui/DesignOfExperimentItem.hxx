//                                               -*- C++ -*-
/**
 *  @brief QStandardItem, observer of designs of experiments
 *
 *  Copyright 2015-2016 EDF-Phimeca
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
 *  You should have received a copy of the GNU Lesser General Public
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef OTGUI_DESIGNOFEXPERIMENTITEM_HXX
#define OTGUI_DESIGNOFEXPERIMENTITEM_HXX

#include "otgui/DesignOfExperiment.hxx"

#include <QStandardItem>

namespace OTGUI {
class OTGUI_API DesignOfExperimentItem : public QObject, public QStandardItem, public Observer
{
  Q_OBJECT

public:
  DesignOfExperimentItem(const DesignOfExperiment & designOfExperiment);

  DesignOfExperiment getDesignOfExperiment() const;

  void setData(const QVariant & value, int role);
  void updateDesignOfExperiment(const DesignOfExperiment & designOfExperiment);
  virtual void update(Observable * source, const OT::String & message);

signals:
  void designOfExperimentChanged(const DesignOfExperiment&);
  void analysisFinished();
  void analysisBadlyFinished(QString);
  void designOfExperimentRemoved(QStandardItem*);

private:
  DesignOfExperiment designOfExperiment_;
};
}
#endif