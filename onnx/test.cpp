
#if 0
Definition: external/onnx/onnx/common/ir.h

#include <onnx/common/ir.h>
	model.graph
	graph.initializer(i).name()  // conv1_w_0
	graph.initializer_names()
	graph.inputs()
	graph.input(i).name() //
	graph.node(i).name() // Conv

	node.kind() == onnx::Symbol("Conv")
	node.attribute_size()
	node.attribute(i)     //

	onnx::ModelProto model;
	onnx::GraphProto &Graph = model.graph();
	onnx::Graph m_onnxGraph = onnx::ImportModelProto(*optModel)

	onnx::Node
	Auto it = m_onnxGraph->begin();
	onnx::Node * node = *it;
	node.kind()
	node.kind().toString()
	node.inputs(), node.outputs()
	node.input(), node.output() // for single input/output
	node.input(i)

	const std::vector<onnx::Dimension> inDim = node.inputs()[0]->sizes();
	m_inN = inDim[0].dim;
	if (node.hasAttribute(onnx::Symbol("kernel_shape")))
	auto &i = node.is(onnx::Symbol("kernel_shape"));
	m_kH = i[0];

	onnx::NodeProto node
	node.op_type()  "Conv"
	node.output(j)
	Node.attribute(j)

	onnx::Value
	v->uniqueName()
	v->sizes()  // dimension
	v->elemType()
	v->node()
	v->replaceAllUsesWith()

	onnx::Tensor
	tensor->name()

Definition: external/onnx/onnx/common/interned_strings.h
	Symbol
	symbol.toString()

void demo(Module &pModule)
{
	onnx::Graph *graph = pModule.getGraph();
	// onnx::Graph
	graph->name(); // string
	ArrayRef<Value*> inputValues = graph->inputs();
	ArrayRef<Value*> outputValues = graph->outputs();

	for (onnx::Node *n : graph->nodes()) {
	}

	for (onnx::graph_node_list_iterator it = graph->begin(), ie = graph->end();
			 it != ie; ++it) {
		onnx::Node *node = *it;
	}

	// onnx::Node *n
	onnx::Symbol symbol = n->kind();
	for (onnx::Value *v : n->outputs()) {
	}


	// onnx::Value *v;
	v->has_unique_name() && v->uniqueName();
	std::vector<onnx::Dimension> dims = { 10, 3, 227, 227 };
	v->sizes();
	v->setSizes(dims);
	v->replaceAllUsesWith(v_new);
	v->uses();
	v->node();


}
#endif
