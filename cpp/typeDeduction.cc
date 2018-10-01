
template<typename T>
void f(T param);  // para will be a copy of whatever passed in.

template<typename T>
void f2(T && param); // it depends on argument type. to deal with rvalue.

int main()
{
	// array and function names decay into pointers for non-reference type specifiers.
	const char name[] = "one two";
	auto arr1 = name; // const char *
	auto &arr2 = name; // const char (&)[]

	void someFunc(int, double);
	auto func1 = someFunc;  // void (*)(int, double)
	auto &func2 = someFunc; // void (&)(int, double)

	auto x = {1, 2, 3}; // std::initializer_list<int>


	return 0;
}
