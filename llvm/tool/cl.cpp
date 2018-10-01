#include "llvm/Support/CommandLine.h"
#include <iostream>

namespace cl = llvm::cl;

cl::opt<bool> OptEnable("opt",cl::desc("Specify enable opt"));

cl::opt<int> MemSize("memsize",
    cl::desc("Specify your mem size"),
    cl::value_desc("memsize"),
    cl::init(1000),
    cl::Required);

cl::alias MemSizeA("m",
    cl::desc("Alias for -memsize"),
    cl::aliasopt(MemSize));

enum DebugOptYesNo {
  no = 0,
  yes = 1,
};

cl::opt<std::string> OptOutput("output");
cl::opt<DebugOptYesNo> DebugOption("debug_opt",
    cl::values(clEnumVal(yes, "true"), clEnumVal(no, "false")));

int main(int argc, char **argv) {
  cl::ParseCommandLineOptions(argc, argv);
  std::cout << "Mem Size " << MemSize << "\n";
  if (DebugOption == DebugOptYesNo::yes) {
    std::cout << "yes\n";
  } else if (DebugOption == DebugOptYesNo::no) {
    std::cout << "no\n";
  }

  if (OptEnable)
    std::cout << "OptEnable enable\n";
  else
    std::cout << "OptEnable disable\n";

  if (DebugOption) {
    std::cout << "true\n";
  } else {
    std::cout << "false\n";
  }
  if (OptOutput != "") {
    std::cout << "OptOutput is " << OptOutput << "\n";
  }
  const char *s = OptOutput.c_str();
  return 0;
}
