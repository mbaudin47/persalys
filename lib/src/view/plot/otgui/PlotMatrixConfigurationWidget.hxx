//                                               -*- C++ -*-
/**
 *  @brief QWidget to configure plot matrices
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
#ifndef OTGUI_PLOTMATRIXCONFIGURATIONWIDGET_HXX
#define OTGUI_PLOTMATRIXCONFIGURATIONWIDGET_HXX

#include "otgui/PlotMatrixWidget.hxx"
#include "otgui/ListWidgetWithCheckBox.hxx"

#include <QLineEdit>

namespace OTGUI
{

class OTGUI_API PlotMatrixConfigurationWidget : public QWidget
{
  Q_OBJECT

public:
  PlotMatrixConfigurationWidget(PlotMatrixWidget * plotMatrix, QWidget* parent = 0);

public slots:
  void showXY(bool);
  void updateLineEdits();
  void updateTitle();
  void exportPlot();

private:
  PlotMatrixWidget * plotMatrix_;
  QLineEdit * titleLineEdit_;
  ListWidgetWithCheckBox * columnsListWidget_;
  ListWidgetWithCheckBox * rowsListWidget_;
};
}
#endif
