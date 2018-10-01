#include <iostream>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

int main(int ac, char **av) {
  // Declare the supported options.
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help", "produce help message")
    ("level", po::value<int>()->required(), "set level")
    ("value", po::value<int>()->default_value(3), "set value")
    ;

  po::variables_map vm;
  po::store(po::parse_command_line(ac, av, desc), vm);
  po::notify(vm);

  std::cout << "value count: " << vm.count("value") << "\n";
  std::cout << "value : " << vm["value"].as<int>() << "\n";
  if (vm.count("help")) {
    std::cout << desc << "\n";
    return 1;
  }

  if (vm.count("level")) {
    std::cout << "level was set to "
      << vm["level"].as<int>() << ".\n";
  } else {
    std::cout << "level was not set.\n";
  }
  return 0;
}
