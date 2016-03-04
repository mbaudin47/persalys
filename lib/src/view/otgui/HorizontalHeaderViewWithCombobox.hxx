//                                               -*- C++ -*-
/**
 *  @brief QHeaderView with combobox
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
#ifndef OTGUI_HORIZONTALHEADERVIEWWITHCOMBOBOX_HXX
#define OTGUI_HORIZONTALHEADERVIEWWITHCOMBOBOX_HXX

#include "Description.hxx"
#include "Indices.hxx"

#include <QHeaderView>
#include <QShowEvent>
#include <QComboBox>

namespace OTGUI {
class HorizontalHeaderViewWithCombobox : public QHeaderView
{
  Q_OBJECT

public:
  HorizontalHeaderViewWithCombobox(QStringList comboBoxItems, QVector<int> columns, QWidget *parent=0);

  void showEvent(QShowEvent * e);
  void scrollContentsBy(int dx, int dy);
  void fixComboPositions();

public slots:
  void handleSectionResized(int);
  void handleSectionMoved(int, int, int);
  void setHeaderData(int);

private:
  QMap<int, QComboBox *> boxes_;
  QStringList comboBoxItems_;
  QVector<int> columnsWithComboBox_;
};
}
#endif