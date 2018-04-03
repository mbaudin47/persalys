//                                               -*- C++ -*-
/**
 *  @brief QMdiSubWindow for the analysis of data
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
 *  You should have received a copy of the GNU Lesser General Public
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef OTGUI_DATANALYSISWINDOW_HXX
#define OTGUI_DATANALYSISWINDOW_HXX

#include "otgui/ResultWindow.hxx"
#include "otgui/DataAnalysisResult.hxx"
#include "otgui/DoubleSpinBox.hxx"

#ifdef OTGUI_HAVE_PARAVIEW
#include "otgui/PVSpreadSheetViewWidget.hxx"
#endif

#include <QGroupBox>

namespace OTGUI
{
class OTGUI_API DataAnalysisWindow : public ResultWindow
{
  Q_OBJECT

public:
  DataAnalysisWindow(OTguiItem * item, QWidget * parent = 0);

protected:
  void initializeVariablesNames();
  void buildInterface();
  virtual void fillListWidget();
  virtual void fillTabWidget();
  virtual void addSummaryTab();
  void addPDF_CDFTab();
  void addBoxPlotTab();
#ifdef OTGUI_HAVE_PARAVIEW
  virtual void addParaviewWidgetsTabs();
  virtual void addParaviewPlotWidgetsTabs(PVSpreadSheetViewWidget* pvSpreadSheet = 0);
#endif
  void addPlotMatrixTab();
  void addScatterPlotsTab();
  virtual void addTableTab();

public slots:
  void updateSpinBoxes(int indexOutput = 0);
  void probaValueChanged(double proba);
  void quantileValueChanged(double quantile);
  virtual void updateVariablesListVisibility(int indexTab);
signals:
  void stateChanged(int);

protected:
  DesignOfExperiment designOfExperiment_;
  DataAnalysisResult result_;
  QString analysisStopCriteriaMessage_;
  QString analysisErrorMessage_;
  OT::Sample failedInputSample_;
  OT::Sample notEvaluatedInputSample_;
  bool resultsSampleIsValid_;
  QString sampleSizeTitle_;
  QStringList inputNames_;
  QStringList inAxisTitles_;
  QStringList outputNames_;
  QStringList outAxisTitles_;
  bool isConfidenceIntervalRequired_;
  double levelConfidenceInterval_;
  bool showTable_;
  QGroupBox * variablesGroupBox_;
  OTguiListWidget * variablesListWidget_;
  QTabWidget * tabWidget_;
private:
  DoubleSpinBox * probaSpinBox_;
  DoubleSpinBox * quantileSpinBox_;
};
}
#endif
