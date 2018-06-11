//                                               -*- C++ -*-
/**
 *  @brief QStandardItem, observer of a limit state
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
#include "otgui/MeshItem.hxx"

#include "otgui/StudyItem.hxx"

using namespace OT;

namespace OTGUI
{

MeshItem::MeshItem(const PhysicalModel & physicalModel)
  : PhysicalModelItem(physicalModel, "Mesh")
{
  setData(tr("Mesh"), Qt::DisplayRole);
  QFont font;
  font.setWeight(font.weight() + 10);
  setData(font, Qt::FontRole);
  setEditable(false);

  buildActions();
}


void MeshItem::buildActions()
{

  // modify?
}


void MeshItem::update(Observable* source, const String & message)
{
  // emit signals to MeshWindow
  if (message == "meshChanged")
  {
    emit meshChanged();
  }
  else if (message == "physicalModelRemoved")
  {
    emit removeRequested(row());
  }
}
}
