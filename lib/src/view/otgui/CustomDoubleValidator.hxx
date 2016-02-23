//                                               -*- C++ -*-
/**
 *  @brief QDoubleValidator
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
#ifndef OTGUI_CUSTOMDOUBLEVALIDATOR_HXX
#define OTGUI_CUSTOMDOUBLEVALIDATOR_HXX

#include <QDoubleValidator>

namespace OTGUI {
class CustomDoubleValidator : public QDoubleValidator
{
  Q_OBJECT

public:
  CustomDoubleValidator(QObject * parent = 0);

  QValidator::State validate(QString & s, int & i) const;
};
}
#endif