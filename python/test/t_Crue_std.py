#! /usr/bin/env python

from __future__ import print_function
import openturns as ot
import openturns.testing
import otguibase

myStudy = otguibase.OTStudy('myStudy')

Q = otguibase.Input('Q', 0., 'Q (m3/s)', ot.TruncatedDistribution(ot.Gumbel(1./558., 1013.), 0, ot.TruncatedDistribution.LOWER))
Ks = otguibase.Input('Ks', 0., 'Ks (m^(1/3)/s)', ot.TruncatedDistribution(ot.Normal(30.0, 7.5), 0, ot.TruncatedDistribution.LOWER))
Zv = otguibase.Input('Zv', 0., 'Zv (m)', ot.Uniform(49.0, 51.0))
Zm = otguibase.Input('Zm', 0., 'Zm (m)', ot.Uniform(54.0, 56.0))

S = otguibase.Output('S', 0., '', '(Q/(Ks*300.*sqrt((Zm-Zv)/5000)))^(3.0/5.0)+Zv-55.5-3.')

myPhysicalModel = otguibase.PhysicalModel('myPhysicalModel')

myPhysicalModel.addInput(Q)
myPhysicalModel.addInput(Ks)
myPhysicalModel.addInput(Zv)
myPhysicalModel.addInput(Zm)

myPhysicalModel.addOutput(S)

myStudy.addPhysicalModel(myPhysicalModel)

limitState = otguibase.LimitState('limitState1', myPhysicalModel, 'S', ot.Greater(), 0.)

## Monte Carlo ##
montecarlo = otguibase.MonteCarloReliabilityAnalysis('myMonteCarlo', limitState, 10000)
myStudy.addAnalysis(montecarlo)
montecarlo.run()
montecarloResult = montecarlo.getResult()

# Comparaison
openturns.testing.assert_almost_equal(0.0006, montecarloResult.getProbabilityEstimate(), 1e-16)


#TODO : solve problem with TruncatedDistribution before
#script = myStudy.dump()
#print(script)
#exec(script)