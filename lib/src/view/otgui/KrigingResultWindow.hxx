//                                               -*- C++ -*-
/**
 *  @brief QMdiSubWindow for the results of kriging
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
#ifndef OTGUI_KRIGINGRESULTWINDOW_HXX
#define OTGUI_KRIGINGRESULTWINDOW_HXX

#include "otgui/ResultWindow.hxx"
#include "otgui/KrigingAnalysisResult.hxx"

#include <QListWidget>

namespace OTGUI {
class OTGUI_API KrigingResultWindow : public ResultWindow
{
  Q_OBJECT

public:
  KrigingResultWindow(AnalysisItem* item);

protected:
  void setParameters(const Analysis& analysis);
  void buildInterface();

public slots:
  void showHideGraphConfigurationWidget(int indexTab);
  void showHideGraphConfigurationWidget(Qt::WindowStates, Qt::WindowStates);
signals:
  void stateChanged(int);

private:
  KrigingAnalysisResult result_;
  QListWidget * outputsListWidget_;
  QTabWidget * tabWidget_;
};
}
#endif