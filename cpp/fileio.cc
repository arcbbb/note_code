#include <fstream>

void way_1()
{
	const char *fname = "input";
	std::fstream f(fname, std::ios::in | std::ios::binary);
}

void way_2()
{
	const char *fname = "input";
	std::fstream f;
	f.open(fname, std::ios::in | std::ios::binary);
	f.close();
}

int main()
{
	return 0;
}
