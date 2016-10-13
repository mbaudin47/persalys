//                                               -*- C++ -*-
/**
 *  @brief MetaModel implements models created by MetaModelAnalysis
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
#ifndef OTGUI_METAMODEL_HXX
#define OTGUI_METAMODEL_HXX

#include "PhysicalModelImplementation.hxx"

namespace OTGUI {
class OTGUI_API MetaModel : public PhysicalModelImplementation
{
  CLASSNAME;

public:
  /** Default constructor */
  MetaModel(const OT::String & name="Unnamed");
  /** Constructor with parameters */
  MetaModel(const OT::String & name,
            const OT::NumericalMathFunction & function);

  /** Virtual constructor */
  virtual MetaModel * clone() const;

  virtual OT::NumericalMathFunction getFunction() const;
  void setFunction(const OT::NumericalMathFunction & function);

  virtual OT::String getPythonScript() const;

  /** String converter */
  virtual OT::String __repr__() const;

  /** Method save() stores the object through the StorageManager */
  void save(OT::Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(OT::Advocate & adv);

private:
  OT::NumericalMathFunction function_;
};

}
#endif