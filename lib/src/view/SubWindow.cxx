//                                               -*- C++ -*-
/**
 *  @brief Base class for all the windows of otgui
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
#include "otgui/SubWindow.hxx"

#include <QEventLoop>
#include <QOpenGLContext>
#include <QSurfaceFormat>
#include <QOpenGLFunctions_3_2_Core>

namespace OTGUI
{

SubWindow::SubWindow(Item * item, QWidget * parent)
  : QWidget(parent)
  , item_(item)
{
  // connections: signal for SubWindowsStackedWidget
  if (item)
  {
    connect(item, SIGNAL(showWindowRequested()), this, SLOT(showRequest()));
    connect(item, SIGNAL(removeWindowRequested()), this, SLOT(removeRequest()));
  }
}


SubWindow::~SubWindow()
{
  item_ = 0;
  emit graphWindowDeactivated();
}


void SubWindow::showRequest()
{
  emit showWindowRequested(this);
}


void SubWindow::removeRequest()
{
  emit removeWindowRequested(this);
}


bool SubWindow::HaveOpenGL32()
{
  char *pNO_GL = getenv("OTGUI_NO_GL");
  if (pNO_GL)
    return false;

  QSurfaceFormat requestedFormat;
  requestedFormat.setVersion(3, 2);
  QOpenGLContext context;
  context.setFormat(requestedFormat);
  if (!context.create())
    return false;

  return context.versionFunctions<QOpenGLFunctions_3_2_Core>();
}


Item * SubWindow::getItem() const
{
  return item_;
}
}