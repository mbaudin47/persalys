//                                               -*- C++ -*-
/**
 *  @brief QMdiSubWindow to define a probabilistic model
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
#ifndef OTGUI_PROBABILISTICMODELWINDOW_HXX
#define OTGUI_PROBABILISTICMODELWINDOW_HXX

#include "otgui/OTguiSubWindow.hxx"
#include "otgui/ProbabilisticModelItem.hxx"
#include "otgui/PlotWidget.hxx"
#include "otgui/InputTableProbabilisticModel.hxx"
#include "otgui/CorrelationTableModel.hxx"
#include "otgui/GraphConfigurationWidget.hxx"

#include <QGroupBox>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QTableView>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QStackedLayout>

namespace OTGUI {
class ProbabilisticModelWindow : public OTguiSubWindow
{
  Q_OBJECT

public :
  ProbabilisticModelWindow(ProbabilisticModelItem * item);

protected:
  void buildInterface();
  void updateStochasticInputsTable();
  void updatePlots();

public slots:
  void updateDistributionWidgets(const QModelIndex & index);
  void updateDistributionParametersWidgets(const QModelIndex & index);
  void updateTruncationParametersWidgets(const QModelIndex & index);
  void updatePhysicalModel(const PhysicalModel &);
  void updateProbabilisticModel();
  void updateCorrelationTable();
  void setCorrelationTabErrorMessage(const QString &);
  void showHideAdvancedWidgets(bool);
  void distributionParametersChanged();
  void typeDistributionParametersChanged(int);
  void truncationParametersChanged();
  void truncationParametersStateChanged();
  void showHideGraphConfigurationWidget(int indexTab);
  void showHideGraphConfigurationWidget(Qt::WindowStates, Qt::WindowStates);

private:
  PhysicalModel physicalModel_;
  int currentIndexTab_;
  QTableView * inputTableView_;
  InputTableProbabilisticModel * inputTableModel_;
  QStackedLayout * rightSideOfSplitterStackedLayout_;
  QLineEdit * valueForDeterministicVariable_;
  PlotWidget * pdfPlot_;
  PlotWidget * cdfPlot_;
  GraphConfigurationWidget * pdf_cdfPlotsConfigurationWidget_;
  QGroupBox * paramEditor_;
  QVBoxLayout * parameterLayout_;
  QComboBox * selectParametersTypeCombo_;
  QLabel * parameterValuesLabel_[5];
  QLineEdit * parameterValuesEdit_[5];
  QGroupBox * advancedGroup_;
  QWidget * advancedWidgets_;
  QCheckBox * lowerBoundCheckBox_;
  QCheckBox * upperBoundCheckBox_;
  QLineEdit * lowerBoundLineEdit_;
  QLineEdit * upperBoundLineEdit_;
  CorrelationTableModel * correlationTableModel_;
  QTableView * correlationTableView_;
  QLabel * correlationErrorMessage_;
};
}
#endif