#include <vector>
#include <algorithm>
#include <iostream>

// p0 and p1 are the parameters
// a0 and a1 are the arguments

void Foo(int p0, int p1)
{
	// Do things
}

int main()
{
	int a0, a1;
	Foo(a0, a1);
	std::vector<int> nums {3, 4, 2, 8, 15, 267}; // c++14
	auto print = [](const int& n) { std::cout << " " << n; }; // c++14
	std::for_each(nums.begin(), nums.end(), print);
	return 0;
}
