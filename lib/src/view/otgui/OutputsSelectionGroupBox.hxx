//                                               -*- C++ -*-
/**
 *  @brief QGroupBox to select outputs
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
#ifndef OTGUI_OUTPUTSSELECTIONGROUPBOX_HXX
#define OTGUI_OUTPUTSSELECTIONGROUPBOX_HXX

#include "otgui/ListWidgetWithCheckBox.hxx"

#include "openturns/Description.hxx"

#include <QGroupBox>

namespace OTGUI {
class OTGUI_API OutputsSelectionGroupBox : public QGroupBox
{
  Q_OBJECT

public:
  OutputsSelectionGroupBox(const OT::Description& outputsNames, const OT::Description& outputsToAnalyse, QWidget* parent=0);

  QStringList getSelectedOutputsNames() const;

public slots:
signals:
  void outputsSelectionChanged(QStringList);

private:
  ListWidgetWithCheckBox * outputsListWidget_;
};
}
#endif