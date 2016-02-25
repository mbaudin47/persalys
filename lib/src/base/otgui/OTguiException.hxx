//                                               -*- C++ -*-
/**
 *  @brief Class to define otgui Exceptions 
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

#ifndef OTGUI_EXCEPTION_HXX
#define OTGUI_EXCEPTION_HXX

#include "Exception.hxx"

namespace OTGUI {

/*
 *
 * All exceptions defined for the user are listed below
 *
 */

#define NEW_OTGUIEXCEPTION( CName ) class OT_API CName : public OT::Exception   \
  {                                                             \
  public:                                                       \
    CName (const OT::PointInSourceFile & point);                    \
    virtual ~CName () throw();                                  \
    template <class T> CName & operator << (T obj)              \
    {                                                           \
      this->Exception::operator << ( obj );                     \
      return *this;                                             \
    }                                                           \
  }

/**
 * @class PhysicalModelNotValidException
 * @brief Raised when physical model is not valid
 * @internal
 */
NEW_OTGUIEXCEPTION( PhysicalModelNotValidException );

#undef NEW_OTGUIEXCEPTION
}
#endif