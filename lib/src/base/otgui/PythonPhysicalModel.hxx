//                                               -*- C++ -*-
/**
 *  @brief PythonPhysicalModel implements models defined by a Python script
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
#ifndef OTGUI_PYTHONPHYSICALMODEL_HXX
#define OTGUI_PYTHONPHYSICALMODEL_HXX

#include "PhysicalModelImplementation.hxx"

namespace OTGUI {
class PythonPhysicalModel : public PhysicalModelImplementation
{
  CLASSNAME;

public:
  PythonPhysicalModel(const OT::String & name);

  PythonPhysicalModel(const OT::String & name,
                      const InputCollection & inputs,
                      const OutputCollection & outputs,
                      const OT::String & code);

  virtual PythonPhysicalModel * clone() const;

  void setCode(const OT::String & code);
  OT::String getCode() const;
  virtual OT::NumericalMathFunction getFunction();

  OT::String dump() const;

private:
  OT::String code_;
};

}
#endif