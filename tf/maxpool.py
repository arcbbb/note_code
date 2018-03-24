#!/usr/bin/env python
import tensorflow as tf
import numpy as np

# format = NHWC
input = tf.Variable(tf.random_normal([1,54,54,96]))
kernel_shape = [3,3]
strides_shape = [2,2]
pooling=tf.nn.max_pool(input,[1,3,3,1],[1,2,2,1],padding='VALID')
init = tf.global_variables_initializer()
with tf.Session() as sess:
  sess.run(init)
  result = sess.run(pooling)
  print(result.shape)
