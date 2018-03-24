#include <memory>
#include <iostream>

void dump_up(std::unique_ptr<char[]> &p)
{
	for (int i = 0; p[i] != '\0'; i++) {
		std::cout << p[i];
	}
}

int main()
{
	char *s = new char[1024];
	std::unique_ptr<char[]> p(new char[1024]);
	auto q = std::make_unique<char[]>(1024);
	p[0] = 'a';
	p[1] = 'b';
	p[2] = 'c';
	p[3] = '\n';
	p[4] = '\0';
	dump_up(p);

	return 0;
}
