//                                               -*- C++ -*-
/**
 *  @brief base tools
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
#include "otgui/BaseTools.hxx"

#include <boost/locale.hpp>

using namespace OT;

namespace OTGUI
{

Parameters::Parameters()
  : pairsCollection_()
{
}


Parameters::~Parameters()
{
}


void Parameters::add(const String& name, const String& value)
{
  std::pair<String, String> newPair;
  newPair.first = name;
  newPair.second = value;
  pairsCollection_.add(newPair);
}


void Parameters::add(const String& name, const double& value)
{
  add(name, OSS() << value);
}


void Parameters::add(const String& name, const UnsignedInteger& value)
{
  add(name, OSS() << value);
}


void Parameters::add(const String& name, const Point& values)
{
  add(name, GetOTPointStr(values, ";"));
}


UnsignedInteger Parameters::getSize() const
{
  return pairsCollection_.getSize();
}


std::pair<String, String> Parameters::operator[](const UnsignedInteger index) const
{
  return pairsCollection_[index];
}


String Parameters::GetOTSampleStr(const Sample& values)
{
  OSS sampleOss;
  sampleOss << "[";
  for (UnsignedInteger i = 0; i < values.getSize(); ++i)
  {
    sampleOss << "[";
    Point pt(values[i]);
    std::copy(pt.begin(), pt.end(), OSS_iterator<Scalar>(sampleOss, ","));
    sampleOss << "]";
    if (i < (values.getSize() - 1))
      sampleOss << ",\n";
  }
  sampleOss << "]\n";

  return sampleOss;
}


String Parameters::GetOTPointStr(const Point& values, const String& separator)
{
  String valuesStr = "[";
  for (UnsignedInteger i = 0; i < values.getSize(); ++i)
  {
    valuesStr += OSS() << values[i];
    if (i < values.getSize() - 1)
      valuesStr += separator + " ";
  }
  valuesStr += "]";
  return valuesStr;
}


String Parameters::GetOTDescriptionStr(const Description& values, const bool quote)
{
  String valuesStr = "[";
  for (UnsignedInteger i = 0; i < values.getSize(); ++i)
  {
    valuesStr += OSS() << (quote ? "'" : "") << values[i] << (quote ? "'" : "");
    if (i < values.getSize() - 1)
      valuesStr += ", ";
  }
  valuesStr += "]";
  return valuesStr;
}


String Parameters::GetOTBoolCollectionStr(const Interval::BoolCollection& values)
{
  String valuesStr = "[";
  for (UnsignedInteger i = 0; i < values.getSize(); ++i)
  {
    valuesStr += values[i] ? "True" : "False";
    if (i < values.getSize() - 1)
      valuesStr += ", ";
  }
  valuesStr += "]";
  return valuesStr;
}


Description Parameters::GetOTIntervalDescription(const Interval& interval)
{
  Description resu(interval.getDimension());

  for (UnsignedInteger i = 0; i < interval.getDimension(); ++i)
  {
    String intervalStr_i;

    intervalStr_i += (interval.getFiniteLowerBound()[i] ? "[" : "]");
    if (interval.getFiniteLowerBound()[i])
      intervalStr_i += OSS() << interval.getLowerBound()[i];
    else
      intervalStr_i += "-∞";
    intervalStr_i += ", ";
    if (interval.getFiniteUpperBound()[i])
      intervalStr_i += OSS() << interval.getUpperBound()[i];
    else
      intervalStr_i += "+∞";
    intervalStr_i += (interval.getFiniteUpperBound()[i] ? "]" : "[");

    resu[i] = intervalStr_i;
  }
  return resu;
}


String Tools::GetLocaleString(const String& str)
{
  boost::locale::generator g;
  g.locale_cache_enabled(true);
  std::locale loc = g(boost::locale::util::get_system_locale());

  return boost::locale::conv::from_utf<char>(str, loc);;
}
}
