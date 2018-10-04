#!/usr/bin/env python

import struct
import numpy as np

def int8tobyte(i8):
  return struct.pack("b", i8)

def int16tobyte(i16):
  return struct.pack("h", i16)

def ArrayToBin(array, filename):
  f = open(filename, 'w+')
  array.tofile(f)
  f.close()

one = [1]*10
two = [2]*10
a = [one]*3 +  [two]*2
a = np.array(a)
result = np.transpose(a, [1,0])
print result
ArrayToBin(result.astype('int8'), "a.bin")
