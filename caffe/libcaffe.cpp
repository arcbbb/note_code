#include <string>
#include "caffe/caffe.hpp"
#include "caffe/proto/caffe.pb.h"
#include "caffe/layers/memory_data_layer.hpp"
#include "caffe/caffe_support.hpp"

using caffe::Blob;
using caffe::Caffe;
using caffe::Net;
using caffe::Layer;
using caffe::Solver;
using caffe::shared_ptr;
using caffe::string;
using caffe::Timer;
using caffe::vector;
using caffe::NetParameter;

namespace {
void dump_blob_shape(Blob<float> *blob) {
  const vector<int> shape = blob->shape();
  for (int i = 0; i < shape.size(); i++) {
    std::cout << shape[i] << ", ";
  }
  std::cout << std::endl;
}

int blob_cmp(Blob<float> *b1, Blob<float> *b2) {
  int b1_bytes = b1->data()->size();
  int b2_bytes = b2->data()->size();
  const float *b1_f = b1->cpu_data();
  const float *b2_f = b2->cpu_data();
  dump_blob_shape(b1);
  dump_blob_shape(b2);
  if (b1_bytes == b2_bytes) {
    for (int i = 0; i < b1_bytes/sizeof(float); i++) {
      if (b1_f[i] != b2_f[i]) {
        std::cout << "at " << i << ", cmp failed" << std::endl;
        return -1;
      }
    }
  } else {
    std::cout << "b1_bytes: " << b1_bytes
      << ", b2_bytes: " << b2_bytes << std::endl;
    return b1_bytes - b2_bytes;
  }
  return 0;
}

Blob<float> *blob_by_name(Net<float> *net, const std::string &name) {
  return net->blob_by_name(name).get();
}

void blob_dump_data(Blob<float> *blob) {
  const vector<int> shape = blob->shape();
  int batch_size = shape[0];
  int channel = shape[1];
  int height = shape[2];
  int width = shape[3];
  const float *tmp = blob->cpu_data();
  for (int N = 0; N < batch_size; N++) {
    for (int i = 0; i < channel*height*width; i++) {
      if (i%height== 0) std::cout << std::endl;
      std::cout << tmp[N*channel*height*width + i] << " ";
    }
    std::cout << std::endl;
  }
}

void caffe_blobs_backup(vector<Blob<float>*> &srcBlobs,
    std::vector<std::vector<std::shared_ptr<float>>> &all_layer_blobs,
    std::map<void*, std::shared_ptr<float>> &blobDict,
    bool is_top) {

    std::vector<std::shared_ptr<float> > backup_layer_data;
    for (int blobId = 0; blobId < srcBlobs.size(); ++blobId) {
      Blob<float> &backupblob = *srcBlobs[blobId];
      int bytes = backupblob.data()->size();

      std::shared_ptr<float> dst_ptr;
      if (is_top) {
        dst_ptr = {new float[bytes/sizeof(float)],
          std::default_delete<float[]>()};
        blobDict[srcBlobs[blobId]] = dst_ptr;
      } else {
        assert(blobDict.find(srcBlobs[blobId]) != blobDict.end());
        dst_ptr = blobDict[srcBlobs[blobId]];
      }

      const float *src_data = backupblob.cpu_data();
      float *dst_data = dst_ptr.get();
      for (int i = 0; i < bytes/sizeof(float); i++) {
        dst_data[i] = (float) src_data[i];
      }

      backup_layer_data.push_back(dst_ptr);
    }
    all_layer_blobs.push_back(std::move(backup_layer_data));
}

void Net_addMemoryData(NetParameter* param, NetParameter* param_data,
  const vector<int> &input_shape) {
  assert(input_shape.size() == 4);
  param_data->CopyFrom(*param);
  param_data->clear_layer();
  caffe::LayerParameter* layer_param = param_data->add_layer();
  layer_param->set_name("bm_data");
  layer_param->set_type("MemoryData");
  layer_param->add_top("data");
  layer_param->add_top("label");

  caffe::NetStateRule *netrule = layer_param->add_include();
  netrule->set_phase(caffe::TEST);

  layer_param->mutable_memory_data_param()->set_batch_size(input_shape[0]);
  layer_param->mutable_memory_data_param()->set_channels(input_shape[1]);
  layer_param->mutable_memory_data_param()->set_height(input_shape[2]);
  layer_param->mutable_memory_data_param()->set_width(input_shape[3]);

  for (int i = 0; i < param->layer_size(); ++i) {
    const caffe::LayerParameter& layer_param = param->layer(i);
    param_data->add_layer()->CopyFrom(layer_param);
  }
}
}  // namespace

namespace caffe_support {

void caffe_free_net(void *net) {
  Net<float> *caffe_net = (Net<float> *)net;
  delete caffe_net;
}

void *caffe_gen_net(const std::string &caffemodel,
       const std::vector<int> &input_shape) {
  Caffe::set_mode(Caffe::CPU);
  NetParameter* out_param = new NetParameter();

  ReadProtoFromBinaryFile(caffemodel, out_param);
  NetParameter* param_mod = new NetParameter();
  Net_addMemoryData(out_param, param_mod, input_shape);

  int level = 0;
  param_mod->mutable_state()->set_phase(caffe::TEST);
  param_mod->mutable_state()->set_level(level);

  int back = FLAGS_minloglevel;
  // do not show blob content
  FLAGS_minloglevel = 2;
        Net<float> *caffe_net = new Net<float>(*param_mod);
  FLAGS_minloglevel = back;
  return caffe_net;
}

void caffe_inference_input(void *net, float *data, float *label,
    int batch_size,
    std::vector<std::vector<std::shared_ptr<float>>> &all_layer_tops,
    std::vector<std::vector<std::shared_ptr<float>>> &all_layer_bottoms,
    const bool cmp_opt) {
  float iter_loss;
  Net<float> *caffe_net = (Net<float> *)net;

  caffe_net->blob_by_name("data").get()->set_cpu_data(data);
  if (label) {
    caffe_net->blob_by_name("label").get()->set_cpu_data(label);
  }

  if (cmp_opt) {
    caffe_net->Forward(&iter_loss);
  }
  int layer_count = caffe_net->layers().size();
  std::map<void*, std::shared_ptr<float>> blobDict;
  for (int i = 0; i < layer_count; ++i) {
    int layer_id = i;

    // Save all bottoms to all_layer_bottoms before Forward
    vector<Blob<float>*> layer_bottoms = caffe_net->bottom_vecs()[layer_id];
    caffe_blobs_backup(layer_bottoms, all_layer_bottoms, blobDict, false);

    if (!cmp_opt) {
      caffe_net->ForwardFromTo(layer_id, layer_id);
    }

    // Save all tops to all_layer_tops after Forward
    vector<Blob<float>*> layer_tops = caffe_net->top_vecs()[layer_id];
    caffe_blobs_backup(layer_tops, all_layer_tops, blobDict, true);
  }
}

void caffe_inference_memdata(void *net, float *data, float *label, int batch_size) {
  Net<float> *caffe_net = (Net<float> *)net;
  caffe::MemoryDataLayer<float> *dataLayer = (caffe::MemoryDataLayer<float> *)
     (caffe_net->layer_by_name("bm_data").get());
  dataLayer->Reset(data, label, batch_size);
  float iter_loss;
  caffe_net->Forward(&iter_loss);
  return;
}

int caffe_has_blob(void *net, const std::string &name) {
  Net<float> *caffe_net = (Net<float> *)net;
  return caffe_net->has_blob(name);
}

const float *caffe_get_blob(void *net, const std::string &name) {
  return blob_by_name((Net<float> *)net, name)->cpu_data();
}

int caffe_get_bottomBlob_neuron_num(void *net, int layer_id, int layer_input_id) {
  Net<float> *caffe_net = (Net<float> *)net;
  Blob<float> *blob = caffe_net->bottom_vecs()[layer_id][layer_input_id];
  const vector<int> shape = blob->shape();
  int sum;
  if (shape.size() > 0) {
    sum = shape[0];
    for (int i = 1; i < shape.size(); i++) {
      sum *= shape[i];
    }
  } else {
    sum = 0;
  }
  return sum;
}

int caffe_get_topBlob_neuron_num(void *net, int layer_id, int layer_output_id) {
  Net<float> *caffe_net = (Net<float> *)net;
  Blob<float> *blob = caffe_net->top_vecs()[layer_id][layer_output_id];
  const vector<int> shape = blob->shape();
  int sum;
  if (shape.size() > 0) {
    sum = shape[0];
    for (int i = 1; i < shape.size(); i++) {
      sum *= shape[i];
    }
  } else {
    sum = 0;
  }
  return sum;
}

int caffe_get_layer_id(void *net, const std::string &layername) {
  Net<float> *caffe_net = (Net<float> *)net;
  const vector<string> &layer_names = caffe_net->layer_names();
  int layer_id = -1;
  for (int i = 0; i < layer_names.size(); ++i) {
    if (layername == layer_names[i]) {
      layer_id = i;
      break;
    }
  }
  return layer_id;
}
}  // namespace caffe_support
