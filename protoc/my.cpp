#include <iostream>
#include <fstream>
#include <google/protobuf/text_format.h>
#include "my.pb.h"

/*
 *  output.txt
 *
 *  name: "test1"
 *  kernel_size: 5
 *  num_output: 100
 *  list {
 *    name: "o0"
 *    id: 0
 *  }
 *  list {
 *    name: "o1"
 *    id: 1
 *    type: STR
 *  }
 */

int main()
{
	me::MeConv m;

	m.set_name("test1");
	m.set_num_output(100);
	m.add_kernel_size(5);

	me::Obj *o0 = m.add_list();
	o0->set_name("o0");
	o0->set_id(0);

	me::Obj *o1 = m.add_list();
	o1->set_name("o1");
	o1->set_id(1);
	o1->set_type(me::Obj_Type_STR);

	// get default value
	std::cout << o0->type() << std::endl;
	std::cout << o1->type() << std::endl;

	// output plain text
	const char *fn = "output.txt";
	std::ofstream f;
	f.open(fn);
	std::string s;
	google::protobuf::TextFormat::PrintToString(m, &s);
	f << s;
	f.close();
	return 0;
}
