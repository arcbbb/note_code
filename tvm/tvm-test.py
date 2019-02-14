#!/usr/bin/env python

import topi
import tvm

def test1():
  n = tvm.var("n")
  n = 100
  A = tvm.placeholder((n,), name='A')
  B = tvm.placeholder((n,), name='B')
  C = tvm.compute(A.shape, lambda i: A[i] + B[i], name="C")
  D = tvm.compute((n-1,), lambda i: C[i] + C[i+1], name="D")
  
  s = tvm.create_schedule(D.op)
#  cx, = C.op.axis
#  s[C].unroll(cx)
  dx, = D.op.axis
  dxo, dxi = s[D].split(dx, factor=16)
  s[C].compute_at(s[D], dxo)
#  s[D].unroll(dxi)
  print(s.stages)
  print(tvm.lower(s, [A, B, C, D], simple_mode=True))

def test_conv():
  data = tvm.placeholder((1, 3, 224, 224))
  kernel = tvm.placeholder((10, 3, 5, 5))

  with tvm.target.create("llvm"):
    conv = topi.nn.conv2d(data, kernel, strides=1, padding=2, dilation=1)
    s = tvm.create_schedule(conv.op)
    print(tvm.lower(s, [data, kernel], simple_mode=True))

def test_conv_relu():
  data = tvm.placeholder((1, 3, 224, 224))
  kernel = tvm.placeholder((10, 3, 5, 5))

  #with tvm.target.create("cuda"):
  with tvm.target.create("llvm"):
    conv = topi.nn.conv2d(data, kernel, strides=1, padding=2, dilation=1)
    relu = topi.nn.relu(conv)
#    s = topi.generic.nn.schedule_conv2d_nchw(relu)
    s = tvm.create_schedule(relu.op)
    s[conv].compute_inline()
    print(tvm.lower(s, [data, kernel], simple_mode=True))

def intrin_gemv(m, l):
    a = tvm.placeholder((l,), name='a')
    b = tvm.placeholder((m, l), name='b')
    k = tvm.reduce_axis((0, l), name='k')
    c = tvm.compute((m,), lambda i: tvm.sum(a[k] * b[i, k], axis=k), name='c')
    Ab = tvm.decl_buffer(a.shape, a.dtype,
                         name="A",
                         offset_factor=1,
                         strides=[1])
    Bb = tvm.decl_buffer(b.shape, b.dtype,
                         name="B",
                         offset_factor=1,
                         strides=[tvm.var("s1"), 1])
    Cb = tvm.decl_buffer(c.shape, c.dtype,
                         name="C",
                         offset_factor=1,
                         strides=[1])
    def intrin_func(ins, outs):
        ib = tvm.ir_builder.create()
        aa, bb = ins
        cc = outs[0]
        ib.emit(tvm.call_extern("int32", "gemv_update",
                                cc.access_ptr("w"),
                                aa.access_ptr("r"),
                                bb.access_ptr("r"),
                                m, l, bb.strides[0]))
        return ib.get()
    with tvm.build_config(offset_factor=1):
        return tvm.decl_tensor_intrin(c.op, intrin_func, binds={a: Ab, b: Bb, c: Cb})

def test_simple_gemm():
  N, M, L = 1024, 512, 64
  A = tvm.placeholder((N, L), name='A')
  B = tvm.placeholder((M, L), name='B')
  k = tvm.reduce_axis((0, L), name='k')
  C = tvm.compute((N, M), lambda i, j: tvm.sum(A[i, k] * B[j, k], axis=k), name='C')

  s = tvm.create_schedule(C.op)
  print(tvm.lower(s, [A, B, C], simple_mode=True))
  print(C.op.body)
  for axis in C.op.axis:
    print(axis)
  for axis in C.op.reduce_axis:
    print(axis)

def test_gemm():
  N, M, L = 1024, 512, 64
  A = tvm.placeholder((N, L), name='A')
  B = tvm.placeholder((M, L), name='B')
  k = tvm.reduce_axis((0, L), name='k')
  C = tvm.compute((N, M), lambda i, j: tvm.sum(A[i, k] * B[j, k], axis=k), name='C')

  s = tvm.create_schedule(C.op)
  x, y = C.op.axis
  z, = C.op.reduce_axis
  factor = 16
  yo, yi = s[C].split(y, factor=factor)
  gemv = intrin_gemv(factor, L)
  s[C].tensorize(yi, gemv)
#  zo, zi = s[C].split(z, factor=16)
#  s[C].reorder(x, yo, zo, yi, zi)

  print(tvm.lower(s, [A, B, C], simple_mode=True))

def main():
#  test1()
#  test_conv()
#  test_conv_relu()
#  test_simple_gemm()
  test_gemm()

main()
