//                                               -*- C++ -*-
/**
 *  @brief The class that implements parametric functions.
 *
 *  Copyright 2005-2018 Airbus-EDF-IMACS-Phimeca
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
#include "otgui/ParametricPointToFieldFunction.hxx"

using namespace OT;

namespace OTGUI
{

CLASSNAMEINIT(ParametricPointToFieldFunction)

/* Default constructor */
ParametricPointToFieldFunction::ParametricPointToFieldFunction ()
  : PointToFieldFunctionImplementation()
  , function_()
  , parametersPositions_()
  , inputPositions_()
  , parameter_()
  , parameterDescription_()
{
}


/* Parameter constructor */
ParametricPointToFieldFunction::ParametricPointToFieldFunction (const PointToFieldFunction & function,
                                        const Indices & set,
                                        const Point & referencePoint)
  : PointToFieldFunctionImplementation(function.getInputDimension(), function.getOutputMesh(), function.getOutputDimension())
  , function_(function)
  , parametersPositions_()
  , inputPositions_()
  , parameter_()
  , parameterDescription_()
{
  const UnsignedInteger inputDimension = function.getInputDimension();
  const UnsignedInteger setDimension = set.getSize();

  // Check if the given parameters positions are compatible with the input dimension of the function
  if (inputDimension < setDimension)
    throw InvalidArgumentException(HERE) << "Error: the size of the parameters positions=" << setDimension << " is greater than the input dimension=" << inputDimension << " of the function.";
  // Check if the given indices are valid
  if (!set.check(inputDimension))
    throw InvalidArgumentException(HERE) << "Error: the given set of positions contain either duplicate positions or positions greater than the input dimension of the function.";

  // Deduce the input position from the input dimension of the function and the parameters positions
  // Warning! the parameters positions can be in any order
  Indices fullIndices(inputDimension);
  Indices otherSet(0);
  fullIndices.fill();
  // Flag by inputDimension all the positions already used by the parameters
  for (UnsignedInteger i = 0; i < setDimension; ++i)
    fullIndices[set[i]] = inputDimension;
  // Then gather all the available indices
  for (UnsignedInteger i = 0; i < inputDimension; ++i)
    if (fullIndices[i] != inputDimension)
      otherSet.add(i);

  parametersPositions_ = set;
  inputPositions_ = otherSet;

  const UnsignedInteger parametersSize = parametersPositions_.getSize();
  // Check if the given reference point has a dimension compatible with the function
  if (referencePoint.getDimension() != parametersSize)
    throw InvalidArgumentException(HERE) << "Error: the given reference point dimension=" << referencePoint.getDimension() << " does not match the parameters size=" << parametersSize;

  // Set the relevant part of the reference point in the parameters
  const Description functionInputDescription(function.getInputDescription());
  Description parameterDescription(parametersSize);
  for (UnsignedInteger i = 0; i < parametersSize; ++ i)
    parameterDescription[i] = functionInputDescription[parametersPositions_[i]];

  parameter_ = referencePoint;
  parameterDescription_ = parameterDescription;

  // And finally the input/output descriptions
  Description inputDescription(0);
  for (UnsignedInteger i = 0; i < inputPositions_.getSize(); ++i)
    inputDescription.add(functionInputDescription[inputPositions_[i]]);
  setInputDescription(inputDescription);
  setOutputDescription(function_.getOutputDescription());
}


/* Virtual constructor */
ParametricPointToFieldFunction * ParametricPointToFieldFunction::clone() const
{
  return new ParametricPointToFieldFunction(*this);
}


/* Evaluation operator */
Sample ParametricPointToFieldFunction::operator() (const Point & point) const
{
  const UnsignedInteger inputDimension = function_.getInputDimension();
  const UnsignedInteger pointDimension = point.getDimension();
  const UnsignedInteger parametersDimension = parametersPositions_.getSize();
  if (pointDimension + parametersDimension != inputDimension)
    throw InvalidArgumentException(HERE) << "Error: expected a point of dimension=" << inputDimension - parametersDimension << ", got dimension=" << pointDimension;
  Point x(inputDimension);
  for (UnsignedInteger i = 0; i < parametersDimension; ++i)
    x[parametersPositions_[i]] = parameter_[i];
  for (UnsignedInteger i = 0; i < pointDimension; ++i)
    x[inputPositions_[i]] = point[i];

  const Sample value(function_(x));
  callsNumber_.increment();
  return value;
}


/* Operator () */
ProcessSample ParametricPointToFieldFunction::operator() (const Sample & inSample) const
{
  const UnsignedInteger size = inSample.getSize();
  const UnsignedInteger inputDimension = function_.getInputDimension();
  const UnsignedInteger sampleDimension = inSample.getDimension();
  const UnsignedInteger parametersDimension = parametersPositions_.getSize();
  if (sampleDimension + parametersDimension != inputDimension)
    throw InvalidArgumentException(HERE) << "Error: expected a sample of dimension=" << inputDimension - parametersDimension << ", got dimension=" << sampleDimension;

  Sample input(size, inputDimension);
  for (UnsignedInteger i = 0; i < size; ++i)
  {
    for (UnsignedInteger j = 0; j < parametersDimension; ++j)
      input(i, parametersPositions_[j]) = parameter_[j];
    for (UnsignedInteger j = 0; j < sampleDimension; ++j)
      input(i, inputPositions_[j]) = inSample(i, j);
  }
  const ProcessSample output(function_(input));
  callsNumber_.fetchAndAdd(size);
  return output;
}


/* Dimension accessor */
UnsignedInteger ParametricPointToFieldFunction::getInputDimension() const
{
  return inputPositions_.getSize();
}


/* String converter */
String ParametricPointToFieldFunction::__repr__() const
{
  PointWithDescription parameters(parameter_);
  parameters.setDescription(parameterDescription_);

  OSS oss;
  oss << "class=" << PointToFieldFunction::GetClassName()
      << " function=" << function_
      << " parameters positions=" << parametersPositions_
      << " parameters=" << parameters
      << " input positions=" << inputPositions_;
  return oss;
}


String ParametricPointToFieldFunction::__str__(const String & offset) const
{
  PointWithDescription parameters(parameter_);
  parameters.setDescription(parameterDescription_);

  OSS oss(false);
  oss << PointToFieldFunction::GetClassName()
      << "(" << function_
      << ", parameters positions=" << parametersPositions_
      << ", parameters=" << parameters
      << ", input positions=" << inputPositions_ << ")";
  return oss;
}
}