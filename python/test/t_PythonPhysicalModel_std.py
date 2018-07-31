#!/usr/bin/env python

from __future__ import print_function
import openturns as ot
import openturns.testing
import otguibase

myStudy = otguibase.OTStudy('myStudy')

# Model
R = otguibase.Input('R', 0., ot.LogNormalMuSigma(
    300., 30.).getDistribution(), 'Yield strength')
F = otguibase.Input('F', 0., ot.Normal(75000., 5000.), 'Traction load')
G = otguibase.Output('G', 'deviation')
code = 'from math import pi\n\ndef _exec(R, F):\n    G = R-F/(pi*100.0)\n    return G\n'

model = otguibase.PythonPhysicalModel('myPhysicalModel', [R, F], [G], code)
myStudy.add(model)

f = model.getFunction()
print(f([300., 75000.]))

# several outputs
model.setCode(
    'from math import pi\n\ndef _exec(R, F):\n    G =  6*F\n    H = 42.0\n    return G, H\n')
f = model.getFunction()
print(f([300., 75000.]))

# test operator() (sample)
model.setCode(
    'from math import pi\n\ndef _exec(R, F):\n    G = 2*R-F/(pi*100.0)\n    return G\n')
model.setParallel(True)
f = model.getFunction()
print(f([[300., 75000.]]))
print(f([[300., 75000.], [400., 74000.]]))

# model with an error
model.setCode(
    'from math import pi\n\ndef _exec(R, F):\n    G = 2*R-F/(pi*100.0)/0.\n    return G\n')
model.setParallel(True)
f = model.getFunction()
try:
  print(f([[300., 75000.]]))
except Exception as e:
  print("ZeroDivisionError occured: %s" % ("ZeroDivisionError: float division by zero" in str(e)))
  print("Error on the line 4 : %s" % ("line 4" in str(e)))

# script
script = myStudy.getPythonScript()
print(script)
exec(script)
