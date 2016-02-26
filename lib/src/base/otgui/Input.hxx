//                                               -*- C++ -*-
/**
 *  @brief Class to define inputs for the physical models 
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
#ifndef OTGUI_INPUT_HXX
#define OTGUI_INPUT_HXX

#include "Variable.hxx"

#include "Distribution.hxx"
#include "DiracFactory.hxx"

namespace OTGUI {
class OTGUI_API Input : public Variable
{
  CLASSNAME;

public:
  /** Default constructor */
  Input();
  /** Constructor with parameters */
  Input(const OT::String & name, const double & value=0., const OT::String & description="",
        const OT::Distribution & distribution=OT::Dirac());

  /** Virtual constructor */
  virtual Input * clone() const;

  virtual OT::String __repr__() const;

  OT::Distribution getDistribution() const;
  void setDistribution(const OT::Distribution & distribution);

  OT::UnsignedInteger getDistributionParametersType() const;
  void setDistributionParametersType(const OT::UnsignedInteger & distributionParametersType);

  bool isStochastic() const;

  OT::String getDistributionPythonScript() const;
  OT::String getPythonScript() const;

  /** Method save() stores the object through the StorageManager */
  void save(OT::Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(OT::Advocate & adv);

private:
  OT::Distribution distribution_;
  OT::UnsignedInteger distributionParametersType_;
};
typedef OT::Collection<Input> InputCollection;
}
#endif