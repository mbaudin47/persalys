//                                               -*- C++ -*-
/**
 *  @brief Class to define data model
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
#ifndef OTGUI_DATAMODEL_HXX
#define OTGUI_DATAMODEL_HXX

#include "FromFileDesignOfExperiment.hxx"

namespace OTGUI {
class OTGUI_API DataModel : public FromFileDesignOfExperiment
{
  CLASSNAME;

public:
  /** Default constructor */
  DataModel(const OT::String & name="Unnamed");

  /** Constructor with parameters */
  DataModel(const OT::String & name,
            const OT::String & fileName,
            const OT::Indices & inputColumns,
            const OT::Indices & outputColumns=OT::Indices(),
            const OT::Description & inputNames=OT::Description(),
            const OT::Description & outputNames=OT::Description());

  /** Constructor with parameters */
  DataModel(const OT::String & name,
            const OT::NumericalSample & inSample,
            const OT::NumericalSample & outSample);

  /** Virtual constructor */
  virtual DataModel * clone() const;

  virtual void setFileName(const OT::String & fileName);

  OT::Indices getOutputColumns() const;
  virtual void setInputColumns(const OT::Indices & inputColumns);
  void setColumns(const OT::Indices & inputColumns,
                  const OT::Indices & outputColumns,
                  const OT::Description & inputNames=OT::Description(),
                  const OT::Description & outputNames=OT::Description());

  OT::Description getInputNames();
  OT::Description getOutputNames();

  virtual OT::String getPythonScript() const;

  /** Method save() stores the object through the StorageManager */
  void save(OT::Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(OT::Advocate & adv);

private:
  OT::Description inputNames_;
  OT::Description outputNames_;
  OT::Indices outputColumns_;
};
}
#endif