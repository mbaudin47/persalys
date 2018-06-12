//                                               -*- C++ -*-
/**
 *  @brief QAbstractTableModel for samples
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
#include "otgui/SampleTableModel.hxx"

#include "otgui/StudyTreeViewModel.hxx"

#include <openturns/SpecFunc.hxx>

#include <QColor>

using namespace OT;

namespace OTGUI
{

SampleTableModel::SampleTableModel(const Sample & data, QObject * parent)
  : QAbstractTableModel(parent)
  , data_(data)
  , sampleIsValid_(true)
{
  for (UnsignedInteger j = 0; j < data_.getSize(); ++j)
    for (UnsignedInteger i = 0; i < data_.getDimension(); ++i)
      if (!SpecFunc::IsNormal(data_(j, i)))
      {
        sampleIsValid_ = false;
        break;
      }
}


int SampleTableModel::columnCount(const QModelIndex& parent) const
{
  if (data_.getSize())
    return data_.getDimension();
  return 0;
}


int SampleTableModel::rowCount(const QModelIndex& parent) const
{
  return data_.getSize();
}


QVariant SampleTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    return QString::fromUtf8(data_.getDescription()[section].c_str());

  if (orientation == Qt::Horizontal && role == Qt::TextAlignmentRole)
    return Qt::AlignCenter;

  return QAbstractTableModel::headerData(section, orientation, role);
}


bool SampleTableModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant& value, int role)
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
  {
    Description description = data_.getDescription();
    description[section] = value.toString().toStdString();
    data_.setDescription(description);
    emit headerDataChanged(Qt::Horizontal, section, section);
    return true;
  }
  return QAbstractTableModel::setHeaderData(section, orientation, value, role);
}


QVariant SampleTableModel::data(const QModelIndex & index, int role) const
{
  if (!index.isValid())
    return QVariant();

  if (role == Qt::TextAlignmentRole)
    return int(Qt::AlignRight | Qt::AlignVCenter);
  else if (role == Qt::DisplayRole)
    return QString::number(data_(index.row(), index.column()), 'g', StudyTreeViewModel::DefaultSignificantDigits);
  else if (role == Qt::UserRole)
    return data_(index.row(), index.column());
  else if (role == Qt::BackgroundRole)
  {
    if (!SpecFunc::IsNormal(data_(index.row(), index.column())))
      return QColor(Qt::red);
    return QVariant();
  }
  return QVariant();
}


void SampleTableModel::exportData(const QString & fileName)
{
  // write
  data_.exportToCSVFile(fileName.toLocal8Bit().data(), ",");
}


bool SampleTableModel::sampleIsValid()
{
  return sampleIsValid_;
}
}
