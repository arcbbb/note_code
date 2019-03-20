#include <iostream>

const char *lib_name();
const char *version();
int main()
{
	std::cout << version() << "\n";
	std::cout << "good!\n";
	std::cout << lib_name() << "\n";
	return 0;
}
