//                                               -*- C++ -*-
/**
 *  @brief QStandardItem, observer of a limit state
 *
 *  Copyright 2015-2016 EDF
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
#include "otgui/LimitStateItem.hxx"

using namespace OT;

namespace OTGUI {

LimitStateItem::LimitStateItem(const LimitState & limitState)
  : ObserverItem(limitState.getName().c_str(), QString("LimitState"))
  , limitState_(limitState)
{
}


LimitStateItem::~LimitStateItem()
{
}


void LimitStateItem::setData(const QVariant & value, int role)
{
  switch (role)
  {
    case Qt::EditRole:
      limitState_.setName(value.toString().toStdString());
      ObserverItem::setData(value, role);
      break;
  }
}


LimitState LimitStateItem::getLimitState() const
{
  return limitState_;
}


void LimitStateItem::update(Observable* source, const String & message)
{
  if (message == "modelOutputsChanged")
  {
    emit outputChanged();
  }
  else if (message == "outputNameChanged")
  {
    emit outputNameChanged();
  }
  else if (message == "operatorChanged")
  {
    emit operatorChanged();
  }
  else if (message == "thresholdChanged")
  {
    emit thresholdChanged();
  }
}
}