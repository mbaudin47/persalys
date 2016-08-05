//                                               -*- C++ -*-
/**
 *  @brief QMdiSubWindow for the results of SobolAnalysis
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
#ifndef OTGUI_SOBOLRESULTWINDOW_HXX
#define OTGUI_SOBOLRESULTWINDOW_HXX

#include "otgui/ResultWindow.hxx"
#include "otgui/SobolResult.hxx"
#include "otgui/GraphConfigurationWidget.hxx"
#include "otgui/CustomStandardItemModel.hxx"

namespace OTGUI {
class SobolResultWindow : public ResultWindow
{
  Q_OBJECT

public:
  SobolResultWindow(AnalysisItem * item);

protected:
  void setParameters(const Analysis & analysis);
  void buildInterface();

public slots:
  void updateIndicesPlot(int section, Qt::SortOrder order);
  void showHideGraphConfigurationWidget(int indexTab);
  void showHideGraphConfigurationWidget(Qt::WindowStates, Qt::WindowStates);

private:
  SobolResult result_;
  QTabWidget * tabWidget_;
  QVector<CustomStandardItemModel*> listTableModels_;
  GraphConfigurationWidget * plotsConfigurationWidget_;
  QVector<PlotWidget*> listPlotWidgets_;
};
}
#endif