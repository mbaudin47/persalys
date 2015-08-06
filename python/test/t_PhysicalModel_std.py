#! /usr/bin/env python

from __future__ import print_function
import openturns as ot
import otguibase

a = otguibase.PhysicalModel()
print(a)

p = ot.NumericalPoint([2, 3])
print(p)

squared_p = a.square(p)
print(squared_p)
