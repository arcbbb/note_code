#define CPU_ONLY

#include <string>
#include "caffe/caffe.hpp"
#include "caffe/proto/caffe.pb.h"
#include "caffe/layers/memory_data_layer.hpp"

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

// g++ test.cpp -L /opt/caffe/build/install/lib  -I /opt/caffe/build/install/include/  -lcaffe -lboost_system -lprotobuf -lglog

void dump_blob_shape(Blob<float> *blob)
{
	const vector<int> shape = blob->shape();
	for (int i = 0; i < shape.size(); i++) {
		std::cout << shape[i] << ", ";
	}
	std::cout << std::endl;
}

int blob_cmp(Blob<float> *b1, Blob<float> *b2)
{
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
		std::cout << "b1_bytes: " << b1_bytes << ", b2_bytes: " << b2_bytes << std::endl;
		return b1_bytes - b2_bytes;
	}
	return 0;
}

Blob<float> *blob_by_name(Net<float> *net, const std::string name)
{
	return net->blob_by_name(name).get();
}

void blob_dump_data(Blob<float> *blob)
{
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

Net<float> *normal_caffe(std::string FLAGS_model, std::string FLAGS_weights)
{
        Caffe::set_mode(Caffe::CPU);
	int back = FLAGS_minloglevel;
	FLAGS_minloglevel = 2;
        Net<float> *caffe_net = new Net<float>(FLAGS_model, caffe::TEST);
	FLAGS_minloglevel = back;
        caffe_net->CopyTrainedLayersFrom(FLAGS_weights);
        float iter_loss;
        const vector<Blob<float>*>& result = caffe_net->Forward(&iter_loss);
	std::cout << "loss = " << iter_loss << std::endl;
	return caffe_net;
}

void Net_addInput(NetParameter* param, NetParameter* param_data)
{
	param_data->CopyFrom(*param);
	param_data->clear_layer();
	caffe::LayerParameter* input1 = param_data->add_layer();
	input1->set_name("bm_data");
	input1->set_type("Input");
	input1->add_top("data");
	input1->add_top("label");

	for (int i = 0; i < param->layer_size(); ++i) {
		const caffe::LayerParameter& layer_param = param->layer(i);
		param_data->add_layer()->CopyFrom(layer_param);
	}
}

void Net_addMemoryData(NetParameter* param, NetParameter* param_data)
{
	param_data->CopyFrom(*param);
	param_data->clear_layer();
	caffe::LayerParameter* layer_param = param_data->add_layer();
	layer_param->set_name("bm_data");
	layer_param->set_type("MemoryData");
	layer_param->add_top("data");
	layer_param->add_top("label");
	caffe::NetStateRule *netrule = layer_param->add_include();
	netrule->set_phase(caffe::TEST);
	layer_param->mutable_memory_data_param()->set_batch_size(100);
	layer_param->mutable_memory_data_param()->set_channels(1);
	layer_param->mutable_memory_data_param()->set_height(28);
	layer_param->mutable_memory_data_param()->set_width(28);

	for (int i = 0; i < param->layer_size(); ++i) {
		const caffe::LayerParameter& layer_param = param->layer(i);
		param_data->add_layer()->CopyFrom(layer_param);
	}
}

Net<float> *model_caffe(Blob<float> *data_blob, std::string FLAGS_weights)
{
        Caffe::set_mode(Caffe::CPU);
	NetParameter* out_param = new NetParameter();
	int level = 0;
	ReadProtoFromBinaryFile(FLAGS_weights, out_param);
	NetParameter* param_mod = new NetParameter();
	Net_addMemoryData(out_param, param_mod);
	//Net_addInput(out_param, param_mod);
	// delete out_param;
	// delete param_mod;
	param_mod->mutable_state()->set_phase(caffe::TEST);
	param_mod->mutable_state()->set_level(level);
	// std::cout << param_mod->DebugString();
#if 1
	int back = FLAGS_minloglevel;
	// do not show blob content
	FLAGS_minloglevel = 2;
        Net<float> *caffe_net = new Net<float>(*param_mod);
	FLAGS_minloglevel = back;
#else
        Net<float> *caffe_net = NULL;
#endif
	// insert data blob
	const vector<int> shape = data_blob->shape();
	int batch_size = shape[0];
	int channel = shape[1];
	int height = shape[2];
	int width = shape[3];

	std::cout << "data_blob shape<" << batch_size << ", " << channel << ", "
		<< height << ", " << width << ">" << std::endl;

	float *data = new float[batch_size * channel * height * width];
	float *label = new float[batch_size];

	std::cout << "data blob size = " << data_blob->data()->size() << std::endl;
	std::cout << "copied size = " << batch_size*channel*height*width*sizeof(float) << std::endl;

	//blob_dump_data(data_blob);

	memcpy(data, data_blob->cpu_data(), batch_size*channel*height*width*sizeof(float));

	caffe::MemoryDataLayer<float> *dataLayer = (caffe::MemoryDataLayer<float> *) (caffe_net->layer_by_name("bm_data").get());
	dataLayer->Reset(data, label, batch_size);
        float iter_loss;
        const vector<Blob<float>*>& result = caffe_net->Forward(&iter_loss);
	std::cout << "loss = " << iter_loss << std::endl;
	return caffe_net;
}

void dump_lenet(Net<float> *net)
{
        Blob<float> *data_blob = blob_by_name(net, "data");
        float *mlabel = blob_by_name(net, "label")->mutable_cpu_data();

	dump_blob_shape(data_blob);
	dump_blob_shape(blob_by_name(net, "ip2"));
	dump_blob_shape(blob_by_name(net, "label"));

	const float *cdata = net->blob_by_name("ip2").get()->cpu_data();
	for (int n = 0; n < 100; n++) {
		std::cout << mlabel[n] << std::endl;
		for (int i = 0; i < 10; i++) {
			std::cout << cdata[n*10+i] << ", ";
		}
		std::cout << std::endl;
	}
}

int main()
{
        std::string dir = "lenet/";
        std::string FLAGS_model = dir + "deploy.prototxt";
        std::string FLAGS_weights = dir + "lenet_iter_10000.caffemodel";
	Net<float> *net1 = normal_caffe(FLAGS_model, FLAGS_weights);
	//dump_lenet(net1);

        Blob<float> *data_blob = blob_by_name(net1, "data");
	Net<float> *net2 = model_caffe(data_blob, FLAGS_weights);
        Blob<float> *ans1 = blob_by_name(net1, "ip2");
        Blob<float> *ans2 = blob_by_name(net2, "ip2");

	// cmp net1 & net2 result
	if (blob_cmp(ans1, ans2)) {
		std::cout << "cmp: ans1 != ans2" << std::endl;
	} else {
		std::cout << "cmp: ans1 == ans2" << std::endl;
	}
        return 0;
}
