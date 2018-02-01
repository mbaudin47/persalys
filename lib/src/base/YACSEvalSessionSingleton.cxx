//                                               -*- C++ -*-
/**
 *  @brief YACSEvalSessionSingleton allocates an YACSEvalSession
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
#include "otgui/YACSEvalSessionSingleton.hxx"

namespace OTGUI
{

YACSEvalSessionSingleton * YACSEvalSessionSingleton::singleton_ = 0;

/* Default constructor */
YACSEvalSessionSingleton::YACSEvalSessionSingleton()
{
  session_ = new YACSEvalSession;
}


/* Destructor */
YACSEvalSessionSingleton::~YACSEvalSessionSingleton()
{
  delete session_;
  session_ = 0;
}


YACSEvalSession * YACSEvalSessionSingleton::Get()
{
  // singleton built at the first function call
  if(!singleton_)
  {
    singleton_ = new YACSEvalSessionSingleton();
  }
  return singleton_->session_;
}


void YACSEvalSessionSingleton::Reset()
{
  if(singleton_)
  {
    delete singleton_;
    singleton_ = 0;
  }
}
}
