//                                               -*- C++ -*-
/**
 *  @brief Class to define outputs of the physical models
 *
 *  Copyright 2015-2017 EDF-Phimeca
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
#include "otgui/Output.hxx"

#include "openturns/PersistentObjectFactory.hxx"

using namespace OT;

namespace OTGUI {

CLASSNAMEINIT(Output);

static Factory<Output> Factory_Output;

/* Default constructor */
Output::Output()
  : Variable()
  , isSelected_(true)
  , hasBeenComputed_(false)
{
}


/* Constructor with parameters */
Output::Output(const String& name, const String& description)
  : Variable(name, 0., description)
  , isSelected_(true)
  , hasBeenComputed_(false)
{
}


/* Virtual constructor */
Output* Output::clone() const
{
  return new Output(*this);
}


bool Output::isSelected() const
{
  return isSelected_;
}


void Output::setIsSelected(const bool isSelected)
{
  isSelected_ = isSelected;
}


bool Output::hasBeenComputed() const
{
  return hasBeenComputed_;
}


void Output::setHasBeenComputed(const bool hasBeenComputed)
{
  hasBeenComputed_ = hasBeenComputed;
}


String Output::getPythonScript() const
{
  OSS oss;
  oss.setPrecision(12);

  String pythonName(getName());
  std::replace(pythonName.begin(), pythonName.end(), '.', '_');
  oss << pythonName << " = otguibase.Output('" << getName() << "', '" << getEscapedDescription() << "')\n";
  if (hasBeenComputed_)
    oss << pythonName << ".setValue(" << getValue() << ")\n";
  if (!isSelected_)
    oss << pythonName << ".setIsSelected(False)\n";

  return oss;
}


/* String converter */
String Output::__repr__() const
{
  OSS oss;
  oss << "class=" << GetClassName()
      << " name=" << getName()
      << " description=" << getDescription();
  return oss;
}


/* Method save() stores the object through the StorageManager */
void Output::save(Advocate & adv) const
{
  Variable::save(adv);
  adv.saveAttribute("isSelected_", isSelected_);
  adv.saveAttribute("hasBeenComputed_", hasBeenComputed_);
}


/* Method load() reloads the object from the StorageManager */
void Output::load(Advocate & adv)
{
  Variable::load(adv);
  adv.loadAttribute("isSelected_", isSelected_);
  adv.loadAttribute("hasBeenComputed_", hasBeenComputed_);
}
}
