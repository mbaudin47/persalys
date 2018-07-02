//                                               -*- C++ -*-
/**
 *  @brief QWidget to configure graphics
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
#ifndef OTGUI_GRAPHCONFIGURATIONWIDGET_HXX
#define OTGUI_GRAPHCONFIGURATIONWIDGET_HXX

#include "otgui/PlotWidget.hxx"
#include "otgui/ValueLineEdit.hxx"

#include <QComboBox>
#include <QButtonGroup>
#include <QCheckBox>

namespace OTGUI
{

class OTGUI_API GraphConfigurationWidget : public QWidget
{
  Q_OBJECT

public:
  enum Type {NoType, Kendall, Scatter, PDF, PDF_Inference, PDFResult, SensitivityIndices, Copula, KSPDF};

  GraphConfigurationWidget(QVector<PlotWidget *> plotWidgets,
                           QStringList inputNames = QStringList(),
                           QStringList outputNames = QStringList(),
                           Type plotType = NoType,
                           QWidget * parent = 0);

  GraphConfigurationWidget(PlotWidget * plotWidget,
                           QStringList inputNames = QStringList(),
                           QStringList outputNames = QStringList(),
                           Type plotType = NoType,
                           QWidget * parent = 0);

  int getCurrentPlotIndex() const;

  QSize minimumSizeHint() const;

protected:
  void buildInterface();

public slots:
  void updateLineEdits();
  void updateYComboBox();
  void plotChanged();
  void updateTitle();
  void updateXLabel();
  void updateYLabel();
  void updateXrange();
  void updateYrange();
  void changeLabelOrientation(int);
  void exportPlot();
signals:
  void currentPlotChanged(int);

private:
  QVector<PlotWidget *> plotWidgets_;
  Type plotType_;
  int currentPlotIndex_;
  QStringList inputNames_;
  QStringList outputNames_;
  QCheckBox * rankCheckBox_;
  QComboBox * distReprComboBox_;
  QComboBox * xAxisComboBox_;
  QComboBox * yAxisComboBox_;
  QLineEdit * titleLineEdit_;
  QLineEdit * xlabelLineEdit_;
  ValueLineEdit * xmin_;
  ValueLineEdit * xmax_;
  QLineEdit * ylabelLineEdit_;
  ValueLineEdit * ymin_;
  ValueLineEdit * ymax_;
};
}
#endif
