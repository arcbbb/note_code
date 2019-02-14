#!/usr/bin/env python

import tvm

def test2stage():
  n = tvm.var("n")
  n = 100
  A = tvm.placeholder((n,), name='A')
  B = tvm.placeholder((n,), name='B')
  C = tvm.compute(A.shape, lambda i: A[i] + B[i], name="C")
  D = tvm.compute((n-1,), lambda i: C[i] + C[i+1], name="D")
  sch = tvm.create_schedule(D.op)
  print(tvm.lower(sch, [A, B], simple_mode=True))

def testInferBound():
  m = 128
  x = tvm.placeholder((m,), name='x')
  y = tvm.placeholder((m,), name='y')
  z = tvm.compute(x.shape, lambda i: x[i] + y[i], name='z')
  
  sch = tvm.create_schedule(z.op)
  sch = sch.normalize()
  dom_map = tvm.schedule.InferBound(sch)
  bounds = dom_map
  stmt = tvm.schedule.ScheduleOps(sch, bounds)
  print stmt
  print(tvm.lower(sch, [x,y], simple_mode=True))

  print "dom_map"
  for k,v in dom_map.items():
    print "key:", k
    print "value:", v

#  finfer = tvm.get_global_func("test.op.InferTensorizeRegion")
#  out_dom, in_dom = finfer(s[z], dom_map)
#  print "out_dom", out_dom
#  print "in_dom", in_dom 
  
def main():
#  testInferBound()
  test2stage()

main()
