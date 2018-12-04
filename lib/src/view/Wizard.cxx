//                                               -*- C++ -*-
/**
 *  @brief Specific QWizard for otgui
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
#include "otgui/Wizard.hxx"

#include <QIcon>

namespace OTGUI
{

Wizard::Wizard(QWidget * parent)
  : QWizard(parent)
{
  setWindowIcon(QIcon(":/images/otgui.ico"));
  setWizardStyle(QWizard::MacStyle);
  setPixmap(QWizard::BackgroundPixmap, QPixmap(":/images/OT_image256x256.png"));
  setButtonText(QWizard::BackButton, tr("Back"));
  setButtonText(QWizard::FinishButton, tr("Finish"));
  setButtonText(QWizard::CancelButton, tr("Cancel"));
  setOption(QWizard::NoDefaultButton, true);
  setOption(QWizard::NoBackButtonOnStartPage, true);
  resize(800, 600);
}
}