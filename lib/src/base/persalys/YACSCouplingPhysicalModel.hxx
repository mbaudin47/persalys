//                                               -*- C++ -*-
/**
 *  @brief Coupling model through YACS
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
#ifndef PERSALYS_YACSCOUPLINGPHYSICALMODEL_HXX
#define PERSALYS_YACSCOUPLINGPHYSICALMODEL_HXX

#include "persalys/CouplingPhysicalModel.hxx"
#include "persalys/YACSEvaluation.hxx"

namespace PERSALYS
{
class PERSALYS_API YACSCouplingPhysicalModel : public CouplingPhysicalModel
{
  CLASSNAME

public:
  /* Default constructor */
  YACSCouplingPhysicalModel(const OT::String & name = "Unnamed",
                            const CouplingStepCollection & steps = CouplingStepCollection());

  /** Virtual constructor */
  virtual YACSCouplingPhysicalModel * clone() const;

  virtual void setCode(const OT::String & code);

  /** Accesor to launching resource properties */
  ydefx::JobParametersProxy& jobParameters();
  const ydefx::JobParametersProxy& jobParameters() const;

  virtual OT::String getPythonScript() const;

  /** Method save() stores the object through the StorageManager */
  void save(OT::Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(OT::Advocate & adv);

  /** String converter */
  virtual OT::String __repr__() const;

  virtual void acceptLaunchParameters(LaunchParametersVisitor* visitor);

protected:

  virtual OT::Function generateFunction(const OT::Description & outputNames) const;
  virtual OT::String getJobParamsPythonScript() const;

private:
  YACSEvaluation evaluation_;
};
}
#endif
