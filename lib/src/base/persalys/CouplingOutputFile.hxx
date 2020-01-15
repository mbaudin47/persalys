//                                               -*- C++ -*-
/**
 *  @brief Output file metadata for coupling
 *
 *  Copyright 2015-2019 EDF-Phimeca
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
#ifndef PERSALYS_COUPLINGOUTPUTFILE_HXX
#define PERSALYS_COUPLINGOUTPUTFILE_HXX

#include "persalys/PersalysPrivate.hxx"
#include <openturns/Description.hxx>
#include <openturns/Point.hxx>

namespace PERSALYS
{
class PERSALYS_API CouplingOutputFile : public OT::PersistentObject
{
  CLASSNAME

public:
  /** Default constructor */
  explicit CouplingOutputFile(const OT::String & path = "");

  /** Virtual constructor */
  virtual CouplingOutputFile * clone() const;

  /** Path accessor */
  void setPath(const OT::String & path);
  OT::String getPath() const;

  /** Variables accessor */
  void setVariables(const OT::Description & variableNames, const OT::Description & tokens,
                    const OT::Point & skipLines, const OT::Point & skipColumns);
  OT::Description getVariableNames() const;
  OT::Description getTokens() const;
  OT::Point getSkipLines() const;
  OT::Point getSkipColumns() const;

  /** String converter */
  virtual OT::String __repr__() const;

  /** Method save() stores the object through the StorageManager */
  void save(OT::Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(OT::Advocate & adv);

private:
  OT::String path_;
  OT::Description variableNames_;
  OT::Description tokens_;
  OT::Point skipLines_;
  OT::Point skipColumns_;
};

}
#endif
