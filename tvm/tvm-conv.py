#!/usr/bin/env python

import tvm

def createMyConv(Input, Filter):

  in_height, in_width = Input.shape
  kernel_h, kernel_w = Filter.shape

  stride_h = 1
  stride_w = 1
  out_h = ((in_height - kernel_h) // stride_h + 1)
  out_w = ((in_width - kernel_w) // stride_w + 1)

  kh = tvm.reduce_axis((0, kernel_h), name='kh')
  kw = tvm.reduce_axis((0, kernel_w), name='kw')

  conv = tvm.compute((out_h, out_w), lambda oh, ow: tvm.sum(Filter[kh, kw] * Input[oh + kh, ow + kw], axis=[kh,kw]), name='c')
  return conv

def intrin_conv(in_h, in_w, kern_h, kern_w):

  in_height = in_h
  in_width = in_w
  kernel_h = kern_h
  kernel_w = kern_w

  stride_h = 1
  stride_w = 1
  out_h = ((in_height - kernel_h) // stride_h + 1)
  out_w = ((in_width - kernel_w) // stride_w + 1)

  Input = tvm.placeholder((in_height, in_width), name='input')
  Filter = tvm.placeholder((kernel_h, kernel_w), name='filter')

  kh = tvm.reduce_axis((0, kernel_h), name='kh')
  kw = tvm.reduce_axis((0, kernel_w), name='kw')
  conv = tvm.compute((out_h, out_w), lambda oh, ow: tvm.sum(Filter[kh, kw] * Input[oh + kh, ow + kw], axis=[kh,kw]), name='c')

  def intrin_func(ins, outs):
    ib = tvm.ir_builder.create()
    inp, filt = ins
    outp = outs[0]
    ib.emit(tvm.call_extern("int32", "inst_conv",
                            outp.access_ptr("w"),
                            inp.access_ptr("r"),
                            filt.access_ptr("r"),
                            ))
    return ib.get()
  with tvm.build_config(offset_factor=1) as cfg:
    scopes = {Input: "local", Filter: "local", conv: "local"}
    binds = {t: tvm.decl_buffer(t.shape, t.dtype, t.op.name,
                                offset_factor=1) for t in [Input, Filter, conv]} 

    return tvm.decl_tensor_intrin(conv.op, intrin_func, binds=binds)

def conv_schedule_0(comp):
  return tvm.create_schedule(comp.op)

def conv_schedule_1(comp):
  sched = tvm.create_schedule(comp.op)
  oh, ow = comp.op.axis
  oho, ohi = sched[comp].split(oh, factor=3)
  sched[comp].unroll(oho)
  return sched

def conv_schedule_2(comp):
  sched = tvm.create_schedule(comp.op)
  oh, ow = comp.op.axis
  conv_inst = intrin_conv(7, 7, 3, 3)
  sched[comp].tensorize(oh, conv_inst)
  return sched

def conv_schedule_3(comp):
  sched = tvm.create_schedule(comp.op)
  oh, ow = comp.op.axis
  oho, ohi = sched[comp].split(oh, factor=3)
  #sched[comp].unroll(oho)
  conv_inst = intrin_conv(5, 7, 3, 3)
  sched[comp].tensorize(ohi, conv_inst)
  return sched

def test_myconv():
  data = tvm.placeholder((7, 7), name='data')
  kernel = tvm.placeholder((3, 3), name='kernel')
  conv = createMyConv(data, kernel)
  print "Infer conv shape: ", conv.shape

  # conv(addr_data, addr_kern, addr_out, ih, iw, kh, kw)
  # conv(0, 0, 0, ih=7, iw=7, kh=3, kw=3) out=5x5
  # -> conv(d_base + 0, k_base, o_base + 0, ih=5, iw=7, kh=3, kw=3) out=3x5
  # -> conv(d_base + 21, k_base, o_base + 15, ih=4, iw=7, kh=3, kw=3) out=2x5

  # schedule
#  sched = conv_schedule_0(conv)
#  sched = conv_schedule_1(conv)
#  sched = conv_schedule_2(conv)
  sched = conv_schedule_3(conv)
  print(tvm.lower(sched, [data, kernel], simple_mode=True))

def main():
  test_myconv()

main()
