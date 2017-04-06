#!/usr/bin/env python

from __future__ import print_function
import openturns as ot
import openturns.testing
import otguibase
import numpy as np

ot.RandomGenerator.SetSeed(0)
ot.TBB_Disable()

myStudy = otguibase.OTStudy('myStudy')
otguibase.OTStudy.Add(myStudy)

## Models
x1 = otguibase.Input('x1', 0.2, ot.Uniform(0., 10.))
x2 = otguibase.Input('x2', 1.2, ot.Uniform(0., 10.))
x3 = otguibase.Input('x3', 1.0)
fake_var = otguibase.Output('fake_var')
fake_var.setIsSelected(False)
y0 = otguibase.Output('y0')
fake_y0 = otguibase.Output('fake_y0')
fake_y0.setIsSelected(False)
y1 = otguibase.Output('y1')

## model 1 ##
formula_fake_var = 'x1'
formula_y0 = 'cos(0.5*x1) + sin(x2)'
formula_y1 = 'cos(0.5*x1) + sin(x2) + x3'
model1 = otguibase.SymbolicPhysicalModel('model1', [x1, x2, x3], [fake_var, y0, fake_y0, y1], [formula_fake_var, formula_y0, formula_y0, formula_y1])
R = ot.CorrelationMatrix(2)
R[0, 1] = 0.25
model1.setCopula(ot.NormalCopula(R))
myStudy.add(model1)

## model 2 ##
code = 'from math import cos, sin\n\ndef _exec(x1, x2, x3):\n    y0 = cos(0.5*x1) + sin(x2) + x3\n    return [y0]\n'
model2 = otguibase.PythonPhysicalModel('model2', [x1, x2, x3], [y0], code)
myStudy.add(model2)

## model 3 ##
filename = 'data.csv'
sample = ot.Normal(3).getSample(200)
sample.stack(ot.Gumbel().getSample(200))
sample.exportToCSVFile(filename, ',')
model3 = otguibase.DataModel('model3', 'data.csv', [0, 2, 3], [1], ['x_0', 'x_2', 'x_3'], ['x_1'])
myStudy.add(model3)

## Designs of Experiment ##

## design 1 ##
ot.RandomGenerator.SetSeed(0)
design_1 = otguibase.DesignOfExperimentImplementation('design_1', model1)
inputSample = ot.LHSExperiment(ot.ComposedDistribution([ot.Uniform(0., 10.), ot.Uniform(0., 10.)]), 10).generate()
inputSample.stack(ot.NumericalSample(10, [0.5]))
design_1.setInputSample(inputSample)
design_1.run()
myStudy.add(design_1)

## design 2 ##
values = [0, 0, 1]
lowerBounds = [0.5, 0.5, 0.9]
upperBounds = [9.5, 9.5, 1.1]
levels = [7, 7, 1]
design_2 = otguibase.FixedDesignOfExperiment('design_2', model1, lowerBounds, upperBounds, levels, values)
myStudy.add(design_2)

## design 3 ##
design_3 = otguibase.FromFileDesignOfExperiment('design_3', model1, 'data.csv', [0, 2, 3])
design_3.run()
myStudy.add(design_3)

## 0- models evaluations

## 0-a evaluation model1 ##
evaluation1 = otguibase.ModelEvaluation('evaluation1', model1)
myStudy.add(evaluation1)

## 0-b evaluation model2 ##
evaluation2 = otguibase.ModelEvaluation('evaluation2', model2)
myStudy.add(evaluation2)

## 1- meta model1 ##

## 1-a Kriging ##
kriging = otguibase.KrigingAnalysis('kriging', design_1)
kriging.setBasis(ot.LinearBasisFactory(2).build())
kriging.setCovarianceModel(ot.MaternModel(2))
kriging.setLeaveOneOutValidation(True)
kriging.setInterestVariables(['y0', 'y1'])
myStudy.add(kriging)

## 1-b Chaos ##
chaos1 = otguibase.FunctionalChaosAnalysis('chaos_1', design_1)
chaos1.setChaosDegree(2)
chaos1.setSparseChaos(True)
chaos1.setInterestVariables(['y1'])
myStudy.add(chaos1)

## 1-c Chaos ##
chaos2 = otguibase.FunctionalChaosAnalysis('chaos_2', design_2)
chaos2.setChaosDegree(2)
chaos2.setSparseChaos(True)
myStudy.add(chaos2)

## 2- central tendancy ##

## 2-a Monte Carlo ##
monteCarlo = otguibase.MonteCarloAnalysis('MonteCarlo', model1)
monteCarlo.setIsConfidenceIntervalRequired(False)
monteCarlo.setMaximumCoefficientOfVariation(-1.)
monteCarlo.setMaximumElapsedTime(1000)
monteCarlo.setMaximumCalls(1000)
monteCarlo.setBlockSize(100)
monteCarlo.setSeed(2)
monteCarlo.setInterestVariables(['y0', 'y1'])
myStudy.add(monteCarlo)

## 2-b Taylor Expansion ##
taylor = otguibase.TaylorExpansionMomentsAnalysis('Taylor', model1)
taylor.setInterestVariables(['y0', 'y1'])
myStudy.add(taylor)
print(taylor)

## 3- reliability ##

## limit state ##
limitState = otguibase.LimitState('aLimitState', model1, 'y1', ot.Greater(), 0.5)
myStudy.add(limitState)

optimAlgo = ot.AbdoRackwitz()
optimAlgo.setMaximumIterationNumber(150)
optimAlgo.setMaximumAbsoluteError(1e-3)

## 3-a Monte Carlo ##
monteCarloReliability = otguibase.MonteCarloReliabilityAnalysis('MonteCarloReliability', limitState)
monteCarloReliability.setMaximumCoefficientOfVariation(-1.)
monteCarloReliability.setMaximumElapsedTime(1000)
monteCarloReliability.setMaximumCalls(1000)
monteCarloReliability.setBlockSize(100)
monteCarloReliability.setSeed(2)
myStudy.add(monteCarloReliability)

## 3-b FORM IS ##
form_is = otguibase.FORMImportanceSamplingAnalysis('FORM_IS', limitState)
form_is.setOptimizationAlgorithm(optimAlgo)
form_is.setMaximumCoefficientOfVariation(-1.)
form_is.setMaximumElapsedTime(1000)
form_is.setMaximumCalls(1000)
form_is.setBlockSize(100)
form_is.setSeed(2)
myStudy.add(form_is)

## 3-c FORM ##
form = otguibase.FORMAnalysis('FORM', limitState)
form.setOptimizationAlgorithm(optimAlgo)
myStudy.add(form)

## 4- sensitivity ##

## 4-a Sobol ##
sobol = otguibase.SobolAnalysis('Sobol', model1)
sobol.setMaximumCoefficientOfVariation(-1)
sobol.setMaximumElapsedTime(1000)
sobol.setMaximumCalls(1000)
sobol.setBlockSize(100)
sobol.setSeed(2)
sobol.setInterestVariables(['y0', 'y1'])
myStudy.add(sobol)

## 4-b SRC ##
src = otguibase.SRCAnalysis('SRC', model1)
src.setSimulationsNumber(1000)
src.setSeed(2)
src.setInterestVariables(['y0', 'y1'])
myStudy.add(src)

## 5- data analysis ##

## 5-a data analysis ##
dataAnalysis = otguibase.DataAnalysis('DataAnalysis', model3)
dataAnalysis.run()
myStudy.add(dataAnalysis)

## 5-b Marginals inference ##
inference = otguibase.InferenceAnalysis('inference', model3)
inference.setInterestVariables(['x_0', 'x_3'])
factories = [ot.NormalFactory(), ot.GumbelFactory()]
inference.setDistributionsFactories('x_3', factories)
inference.setLevel(0.9)
myStudy.add(inference)

## 5-c Copula inference ##
copulaInference = otguibase.CopulaInferenceAnalysis('copulaInference', model3)
factories = [ot.NormalCopulaFactory(), ot.GumbelCopulaFactory()]
copulaInference.setDistributionsFactories(['x_0', 'x_3'], factories)
copulaInference.run()
myStudy.add(copulaInference)