//                                               -*- C++ -*-
/**
 *  @brief YACSPhysicalModel implements models defined by an xml file
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
#include "persalys/YACSPhysicalModel.hxx"
#include "persalys/BaseTools.hxx"

#include <openturns/PersistentObjectFactory.hxx>

using namespace OT;

namespace PERSALYS
{

CLASSNAMEINIT(YACSPhysicalModel)

static Factory<YACSPhysicalModel> Factory_YACSPhysicalModel;

/* Default constructor */
YACSPhysicalModel::YACSPhysicalModel(const String & name)
  : PhysicalModelImplementation(name)
  , evaluation_()
{
  updateData();
}


/* Constructor with parameters */
YACSPhysicalModel::YACSPhysicalModel(const String & name,
                                     const InputCollection & inputs,
                                     const OutputCollection & outputs,
                                     const String & script)
  : PhysicalModelImplementation(name, inputs, outputs)
  , evaluation_(script)
{
  updateData();
}


/* Virtual constructor */
YACSPhysicalModel* YACSPhysicalModel::clone() const
{
  return new YACSPhysicalModel(*this);
}


void YACSPhysicalModel::setInputs(const InputCollection & inputs)
{
  throw NotYetImplementedException(HERE) << "Use setContent to modify a YACS model";
}


void YACSPhysicalModel::addInput(const Input & input)
{
  throw NotYetImplementedException(HERE) << "Use setContent to modify a YACS model";
}


void YACSPhysicalModel::removeInput(const String & inputName)
{
  throw NotYetImplementedException(HERE) << "Use setContent to modify a YACS model";
}


void YACSPhysicalModel::setOutputs(const OutputCollection & outputs)
{
  throw NotYetImplementedException(HERE) << "Use setContent to modify a YACS model";
}


void YACSPhysicalModel::addOutput(const Output & output)
{
  throw NotYetImplementedException(HERE) << "Use setContent to modify a YACS model";
}


void YACSPhysicalModel::removeOutput(const String & outputName)
{
  throw NotYetImplementedException(HERE) << "Use setContent to modify a YACS model";
}


ydefx::JobParametersProxy& YACSPhysicalModel::jobParameters()
{
  return evaluation_.jobParameters();
}


const ydefx::JobParametersProxy& YACSPhysicalModel::jobParameters()const
{
  return evaluation_.jobParameters();
}


String YACSPhysicalModel::getContent() const
{
  return evaluation_.getContent();
}


void YACSPhysicalModel::setContent(const String & script)
{
  try
  {
    evaluation_.setContent(script);
  }
  catch (std::exception & ex)
  {
    throw InvalidArgumentException(HERE) << "Error in the script.\n" << ex.what();
  }
  updateData();
}


void YACSPhysicalModel::updateData()
{
  unsigned int size = evaluation_.getInputDimension();
  InputCollection newInputs(size);
  for (unsigned int i = 0; i < size; ++ i)
  {
    const String inputName(evaluation_.getInputVariablesNames()[i]);
    if (hasInputNamed(inputName))
      newInputs[i] = getInputByName(inputName);
    else
      newInputs[i] = Input(inputName, evaluation_.getInputValues()[i]);
  }
  PhysicalModelImplementation::setInputs(newInputs);

  size = evaluation_.getOutputDimension();
  OutputCollection newOutputs(size);
  for (unsigned int i = 0; i < size; ++ i)
  {
    const String outputName(evaluation_.getOutputVariablesNames()[i]);
    if (hasOutputNamed(outputName))
      newOutputs[i] = getOutputByName(outputName);
    else
      newOutputs[i] = Output(outputName);
  }
  PhysicalModelImplementation::setOutputs(newOutputs);
}


Function YACSPhysicalModel::generateFunction(const Description & outputNames) const
{
  YACSEvaluation anEvaluation(evaluation_);
  anEvaluation.setOutputVariablesNames(outputNames);

  return Function(anEvaluation);
}


static void replaceInString(String& workString,
                            const String& strToReplace,
                            const String& newValue)
{
  std::size_t pos = workString.find(strToReplace);
  while( pos != std::string::npos)
  {
    workString.replace(pos, strToReplace.size(), newValue);
    pos = workString.find(strToReplace, pos+newValue.size());
  }
}

String YACSPhysicalModel::getHtmlDescription(const bool deterministic) const
{
  OSS oss;
  oss << PhysicalModelImplementation::getHtmlDescription(deterministic);
  oss << "<h3>Outputs</h3><p>";
  oss << "<table style=\"width:100%\" border=\"1\" cellpadding=\"5\">";
  oss << "<tr>";
  oss << "  <th>Name</th>";
  oss << "  <th>Description</th>";
  oss << "</tr>";
  for (UnsignedInteger i = 0; i < getOutputDimension(); ++i)
  {
    oss << "<tr>";
    oss << "  <td>" << getOutputNames()[i] << "</td>";
    const String desc(getOutputs()[i].getDescription());
    oss << "  <td>" << (desc.empty() ? "-" : desc) << "</td>";
    oss << "</tr>";
  }
  oss << "</table></p>";
  oss << "</table></p>";
  oss << "<h3>Content</h3>";
  oss << "<pre>";
  String code = getContent();
  // replace all "<" by "&lt;"
  replaceInString(code, "<", "&lt;");
  oss << code;
  oss << "</pre>";

  return oss;
}


String YACSPhysicalModel::getPythonScript() const
{
  OSS oss;

  for (UnsignedInteger i = 0; i < getInputDimension(); ++ i)
    oss << getInputs()[i].getPythonScript();

  for (UnsignedInteger i = 0; i < getOutputDimension(); ++ i)
    oss << getOutputs()[i].getPythonScript();

  oss << "inputs = " << Parameters::GetOTDescriptionStr(getInputNames(), false) << "\n";
  oss << "outputs = " << Parameters::GetOTDescriptionStr(getOutputNames(), false) << "\n";

  // replace ''' by """
  std::string myString = getContent();
  replaceInString(myString, "'''", "\"\"\"");

  oss << "code = '''" << myString << "'''\n";
  oss << getName()
      << " = persalys.YACSPhysicalModel('" << getName() << "'"
      << ", inputs, outputs, code)\n";
  oss << getJobParamsPythonScript();

  oss << getProbaModelPythonScript();
  oss << PhysicalModelImplementation::getCopulaPythonScript();

  return oss;
}


OT::String YACSPhysicalModel::getJobParamsPythonScript() const
{
  OSS oss;
  std::list<std::string> inFiles = jobParameters().in_files();
  if(!inFiles.empty())
  {
    String filesListName = getName() + "_infiles";
    oss << filesListName << " = [";
    for(const std::string& fileName : inFiles)
      oss << "'" << fileName << "',\n";
    oss << "]\n";
    oss << getName() << ".jobParameters().salome_parameters.in_files = "
        << filesListName << "\n";
  }
  return oss;
}


/** String converter */
String YACSPhysicalModel::__repr__() const
{
  OSS oss;
  oss << PhysicalModelImplementation::__repr__()
      << " evaluation=" << evaluation_;
  return oss;
}


/* Method save() stores the object through the StorageManager */
void YACSPhysicalModel::save(Advocate & adv) const
{
  PhysicalModelImplementation::save(adv);
  adv.saveAttribute("evaluation_", evaluation_);
}


/* Method load() reloads the object from the StorageManager */
void YACSPhysicalModel::load(Advocate & adv)
{
  PhysicalModelImplementation::load(adv);
  adv.loadAttribute("evaluation_", evaluation_);
}


void YACSPhysicalModel::acceptLaunchParameters(LaunchParametersVisitor* visitor)
{
  visitor->visitYACS(this);
}
}
