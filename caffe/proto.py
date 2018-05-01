
import caffe
from caffe.proto import caffe_pb2
from google.protobuf import text_format

# caffemodel reader

dir = '/model/lenet/'
deploy_proto = dir + 'deploy.prototxt'
caffe_model = dir + 'lenet_iter_10000.caffemodel'
#caffe_model = dir + 'bmnet_lenet_iter_10000_int8.10x100.caffemodel'
net = caffe.Net(deploy_proto, caffe_model, caffe.TEST)
for blob in net.blobs:
    print blob
net.blobs['data'].data.flatten()
net.params['conv1'][0].data.shape # weight
net.params['conv1'][1].data.shape # bias
#net.blobs['data'].data[...] = data
net.forward(start='data', end='conv1')
data = net.blobs['conv1'].data

# protobuf reader
parsible_net = caffe_pb2.NetParameter()
text_format.Merge(open(deploy_proto).read(), parsible_net)
for layer in parsible_net.layer:
    print "name " + layer.name + ", type " + layer.type
    for bottom in layer.bottom:
        print "bottom " + bottom
    for top in layer.top:
        print "top " + top
