//                                               -*- C++ -*-
/**
 *  @brief QMdiSubWindow for the analysis of data
 *
 *  Copyright 2015-2017 EDF-Phimeca
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
#ifndef OTGUI_DATANALYSISWINDOW_HXX
#define OTGUI_DATANALYSISWINDOW_HXX

#include "otgui/ResultWindow.hxx"
#include "otgui/DataAnalysisResult.hxx"
#include "otgui/DoubleSpinBox.hxx"

#include <QGroupBox>

namespace OTGUI {
class OTGUI_API DataAnalysisWindow : public ResultWindow
{
  Q_OBJECT

public:
  DataAnalysisWindow(AnalysisItem * item, QWidget * parent=0);

protected:
  virtual void initialize(AnalysisItem * item) = 0;
  void buildInterface();
  void addSummaryTab();
  void addPDF_CDFTab();
  void addBoxPlotTab();
#ifdef OTGUI_HAVE_PARAVIEW
  void addParaviewWidgetsTabs();
#endif
  void addPlotMatrixTab();
  void addScatterPlotsTab();
  void addTableTab();

public slots:
  void updateSpinBoxes(int indexOutput=0);
  void probaValueChanged(double proba);
  void quantileValueChanged(double quantile);
  void updateVariablesListVisibility(int indexTab);
signals:
  void stateChanged(int);

protected:
  DataAnalysisResult result_;
  bool resultsSampleIsValid_;
  QString sampleSizeTitle_;
  QStringList stochInputNames_;
  QStringList inAxisTitles_;
  QStringList outputNames_;
  QStringList outAxisTitles_;
  bool isConfidenceIntervalRequired_;
  double levelConfidenceInterval_;
  bool showTable_;
private:
  QGroupBox * variablesGroupBox_;
  OTguiListWidget * variablesListWidget_;
  QTabWidget * tabWidget_;
  DoubleSpinBox * probaSpinBox_;
  DoubleSpinBox * quantileSpinBox_;
};
}
#endif
