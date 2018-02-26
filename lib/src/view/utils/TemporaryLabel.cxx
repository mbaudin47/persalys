//                                               -*- C++ -*-
/**
 *  @brief QLabel with temporary message
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
 *  You should have received a copy of the GNU Lesser General Public
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "otgui/TemporaryLabel.hxx"

namespace OTGUI
{

TemporaryLabel::TemporaryLabel(QWidget *parent)
  : QLabel(parent)
  , qtimelineList_()
{
  setWordWrap(true);
}


void TemporaryLabel::setTemporaryErrorMessage(const QString& message)
{
  if (message.isEmpty())
    return;
  setText(QString("<font color=red>%1</font>").arg(message));
  QTimeLine * time = new QTimeLine(7000, this);
  qtimelineList_.push_back(time);
  connect(time, SIGNAL(stateChanged(QTimeLine::State)), this, SLOT(reInitErrorMessage(QTimeLine::State)));
  time->start();
}


void TemporaryLabel::reInitErrorMessage(QTimeLine::State state)
{
  if (state == QTimeLine::NotRunning)
  {
    if (qtimelineList_.isEmpty())
      return;
    qtimelineList_.removeFirst();
    // if another QTimeLine started before the end of the previous one: do nothing
    if (qtimelineList_.size())
      return;
    // else initialize error message
    setText("");
  }
}
}