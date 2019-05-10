#!/usr/bin/env python

from __future__ import print_function
import openturns as ot
import openturns.testing
import otguibase

myStudy = otguibase.Study('myStudy')

# Model
dist_Q = ot.Normal(10200, 100)
dist_E = ot.Normal(3000, 15)
dist_C = ot.Normal(4000, 60)

Q = otguibase.Input('Q', 10200, dist_Q, 'Primary energy (W.h)')
E = otguibase.Input('E', 3000, dist_E, 'Produced electric energy (W.h)')
C = otguibase.Input('C', 4000, dist_C, 'Valued thermal energy (W.h)')
Ep = otguibase.Output('Ep', 'Primary energy savings (W.h)')

model = otguibase.SymbolicPhysicalModel(
    'myPhysicalModel', [Q, E, C], [Ep], ['1-(Q/((E/((1-0.05)*0.54))+(C/0.8)))'])
myStudy.add(model)

outputSample = [[0.060036508072],
                [0.0292238907867],
                [0.0684295269203],
                [0.0378920382755],
                [0.0812679132055],
                [0.0511512752497],
                [0.0892876773294],
                [0.0594339324804]]


# Design of Experiment - Parametric analysis ##
aDesign = otguibase.GridDesignOfExperiment('aDesign_0', model)
aDesign.setLevels([2, 2, 2])
myStudy.add(aDesign)
aDesign.run()

# Comparaison
openturns.testing.assert_almost_equal(
    outputSample, aDesign.getResult().getDesignOfExperiment().getOutputSample(), 1e-16)

# Taylor Expansions ##
taylorExpansionsMoments = otguibase.TaylorExpansionMomentsAnalysis(
    'myTaylorExpansionMoments', model)
myStudy.add(taylorExpansionsMoments)
taylorExpansionsMoments.run()
taylorExpansionsMomentsResult = taylorExpansionsMoments.getResult()

# Comparaison
openturns.testing.assert_almost_equal(
    0.059730458221, taylorExpansionsMomentsResult.getMeanFirstOrder()[0], 1e-13)

# Monte Carlo ##
montecarlo = otguibase.MonteCarloAnalysis('myMonteCarlo', model)
montecarlo.setMaximumCalls(1000)
montecarlo.setMaximumCoefficientOfVariation(-1)
myStudy.add(montecarlo)
montecarlo.run()
montecarloResult = montecarlo.getResult()

# Comparaison
openturns.testing.assert_almost_equal(
    0.0597109963361, montecarloResult.getMean()[3][0], 1e-13)
openturns.testing.assert_almost_equal(
    0.0114128746587, montecarloResult.getStandardDeviation()[3][0], 1e-13)

meanCI = montecarloResult.getMeanConfidenceInterval()
openturns.testing.assert_almost_equal(
    0.0590036320343, meanCI.getLowerBound()[3], 1e-13)
openturns.testing.assert_almost_equal(
    0.0604183606379, meanCI.getUpperBound()[3], 1e-13)
stdCi = montecarloResult.getStdConfidenceInterval()
openturns.testing.assert_almost_equal(
    0.0109336748621, stdCi.getLowerBound()[3], 1e-13)
openturns.testing.assert_almost_equal(
    0.0119363302339, stdCi.getUpperBound()[3], 1e-13)

# Sobol ##
sobol = otguibase.SobolAnalysis('mySobol', model)
sobol.setReplicationSize(200)
sobol.setMaximumCalls(1000)
myStudy.add(sobol)
sobol.run()
sobolResult = sobol.getResult()

# Comparaison
firstOrderIndicesValues = [
    [ 0.643987, 0.0183602, 0.255834 ]]
totalIndicesValues = [
    [ 0.610267, 0.0494237, 0.280706 ]]
openturns.testing.assert_almost_equal(
    firstOrderIndicesValues, sobolResult.getFirstOrderIndices(), 1e-6)
openturns.testing.assert_almost_equal(
    totalIndicesValues, sobolResult.getTotalIndices(), 1e-6)

# SRC ##
src = otguibase.SRCAnalysis('mySRC', model)
myStudy.add(src)
src.run()
srcResult = src.getResult()

# Comparaison
openturns.testing.assert_almost_equal(
    0.628946, srcResult.getIndices()[0][0], 1e-5)
openturns.testing.assert_almost_equal(
    0.0476118, srcResult.getIndices()[0][1], 1e-5)
openturns.testing.assert_almost_equal(
    0.318226, srcResult.getIndices()[0][2], 1e-5)

# Chaos ##
values = [10200, 3000, 4000]
lowerBounds = [10035.5, 2975.33, 3901.31]
upperBounds = [10364.5, 3024.67, 4098.69]
levels = [10, 10, 10]
design_1 = otguibase.GridDesignOfExperiment(
    'aDesign_1', model, lowerBounds, upperBounds, levels, values)
design_1.run()
myStudy.add(design_1)

chaos = otguibase.FunctionalChaosAnalysis('chaos_0', design_1)
chaos.setChaosDegree(2)
chaos.setSparseChaos(False)
myStudy.add(chaos)
chaos.run()
chaosResult = chaos.getResult()

# Comparaison
openturns.testing.assert_almost_equal(
    0.6356916720224053, chaosResult.getSobolResult().getFirstOrderIndices()[0][0], 1e-16)
openturns.testing.assert_almost_equal(
    0.04806204987068495, chaosResult.getSobolResult().getFirstOrderIndices()[0][1], 1e-17)
openturns.testing.assert_almost_equal(
    0.31620207904361813, chaosResult.getSobolResult().getFirstOrderIndices()[0][2], 1e-17)
openturns.testing.assert_almost_equal(
    0.6357266809805613, chaosResult.getSobolResult().getTotalIndices()[0][0], 1e-16)
openturns.testing.assert_almost_equal(
    0.04807585948286413, chaosResult.getSobolResult().getTotalIndices()[0][1], 1e-17)
openturns.testing.assert_almost_equal(
    0.3162416585998657, chaosResult.getSobolResult().getTotalIndices()[0][2], 1e-17)

# script
script = myStudy.getPythonScript()
print(script)
exec(script)
