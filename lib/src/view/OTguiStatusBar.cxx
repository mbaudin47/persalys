//                                               -*- C++ -*-
/**
 *  @brief QStatusBar
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
#include "otgui/OTguiStatusBar.hxx"

namespace OTGUI {

OTguiStatusBar::OTguiStatusBar(QWidget *parent)
  : QStatusBar(parent)
  , messageLabel_(new QLabel)
{
  addWidget(messageLabel_);
}


void OTguiStatusBar::showErrorMessage(const QString & message)
{
  messageLabel_->setText(QString("%1%2%3").arg("<font color=red>").arg(message).arg("</font>"));
}


void OTguiStatusBar::showMessage(const QString & message)
{
  messageLabel_->setText(message);
}


void OTguiStatusBar::clearMessage()
{
  messageLabel_->setText("");
}
}