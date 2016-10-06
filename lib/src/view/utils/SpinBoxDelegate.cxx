//                                               -*- C++ -*-
/**
 *  @brief QStyledItemDelegate used to define Correlations
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
#include "otgui/SpinBoxDelegate.hxx"

#include "otgui/DoubleSpinBox.hxx"

namespace OTGUI {

SpinBoxDelegate::SpinBoxDelegate(QObject *parent)
  : QStyledItemDelegate(parent)
{
}


QWidget* SpinBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const
{
  DoubleSpinBox * editor = new DoubleSpinBox(parent);
  editor->setFrame(false);
  editor->setMinimum(-1.0);
  editor->setMaximum(1.0);
  editor->setSingleStep(0.05);
  editor->setDecimals(3);

  return editor;
}


void SpinBoxDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
  DoubleSpinBox * spinBox = static_cast<DoubleSpinBox*>(editor);
  spinBox->setValue(index.model()->data(index, Qt::EditRole).toDouble());
}


void SpinBoxDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
{
  DoubleSpinBox * spinBox = static_cast<DoubleSpinBox*>(editor);
  spinBox->interpretText();
  model->setData(index, spinBox->value(), Qt::EditRole);
}


void SpinBoxDelegate::updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex &) const
{
  editor->setGeometry(option.rect);
}
}