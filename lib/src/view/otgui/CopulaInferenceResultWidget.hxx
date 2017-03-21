//                                               -*- C++ -*-
/**
 *  @brief QWidget with a list of copulas and their PDF-CDF/parameters
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
#ifndef OTGUI_COPULAINFERENCERESULTWIDGET_HXX
#define OTGUI_COPULAINFERENCERESULTWIDGET_HXX

#include "otgui/CopulaInferenceSetResult.hxx"
#include "otgui/CustomStandardItemModel.hxx"
#include "otgui/ResizableTableViewWithoutScrollBar.hxx"

namespace OTGUI {
class OTGUI_API CopulaInferenceResultWidget : public QWidget
{
  Q_OBJECT

public:
  CopulaInferenceResultWidget(const CopulaInferenceSetResult& currentSetResult,
                              const OT::NumericalSample& sample,
                              const bool displayPDF=true,
                              QWidget* parent=0);

//   TODO: OT::Distribution getDistribution() const;

protected:
  void buildInterface();

public slots:
  void updateRadioButtonsDistributionTable(const QModelIndex& = QModelIndex());
  void updateParametersTable(const QModelIndex& = QModelIndex());
signals:
  void stateChanged();
  void distributionChanged(int);
  void graphWindowActivated(QWidget*);
  void graphWindowDeactivated();

private:
  CopulaInferenceSetResult currentSetResult_;
  OT::NumericalSample sample_;
  bool displayPDF_;
  ResizableTableViewWithoutScrollBar * distTableView_;
  CustomStandardItemModel * distTableModel_;
};
}
#endif