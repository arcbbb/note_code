#!/usr/bin/env python
import tensorflow as tf
import numpy as np

def NHWC2NCHW(tensor):
  return tf.transpose(tensor, [0,3,1,2])

def NCHW2NHWC(tensor):
  return tf.transpose(tensor, [0,2,3,1])

def NCHW2HWCN(tensor):
  return tf.transpose(tensor, [2,3,1,0])

def HWNC2NCHW(tensor):
  return tf.transpose(tensor, [2,3,0,1])

# <1,1,3,3>
a33 = [[[[1,2,3],[4,5,6],[7,8,9]]]]
a = a33
# <1,1,3,3>
b = [[[[0,0,0],[0,1,0],[0,0,0]]]]
#b = [[[[-1,0,0],[0,1,0],[0,0,0]]]]

input_np = np.array(a)
filter_np = np.array(b)

#input = tf.Variable(tf.random_normal([1,2,2,1]))
#filter = tf.Variable(tf.random_normal([1,1,1,1]))

input_tf = tf.convert_to_tensor(a, np.float32)

filter_tf = tf.convert_to_tensor(filter_np, np.float32)

#
# TF Conv2d:
# input tensor : <N,H,W,IC>
# kernel tensor: <H,W,IC,OC>
#

#input_tf = tf.transpose(input_tf,[0,2,3,1])
input_tf = NCHW2NHWC(input_tf)
filter_tf = NCHW2HWCN(filter_tf)
#filter_tf = tf.transpose(filter_tf,[2,3,0,1])

input = tf.Variable(input_tf, dtype=tf.float32)
print "input shape:", input.get_shape()

filter = tf.Variable(filter_tf, dtype=tf.float32)
print "filter shape:", filter.get_shape()

op = tf.nn.conv2d(input, filter, strides=[1, 1, 1, 1], padding='SAME', data_format='NHWC')
#op = tf.nn.conv2d(input, filter, strides=[1, 1, 1, 1], padding='VALID', data_format='NHWC')
init = tf.global_variables_initializer()
with tf.Session() as sess:
  sess.run(init)

  print("input")
  print(NHWC2NCHW(input).eval())
  print("filter")
  print(HWNC2NCHW(filter).eval())
  print("result")
  result = sess.run(op)
  print(result)
  print(NHWC2NCHW(result).eval())
