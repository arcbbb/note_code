#if 0
template<class T>
void swap(T& a, T& b)
{
	T tmp(a);
	a = b;
	b = tmp;
}
#endif

template<class T>
void swap(T& a, T& b)
{
	T tmp(std::move(a));
	a = std::move(b);
	b = std::move(tmp);
}

Derived(Derived&& rhs)
	// : Base(rhs)         // wrong: rhs is an lvalue
	: Base(std::move(rhs)) // good, calls Base(Base&& rhs)
{
	  // Derived-specific stuff
}

X foo()
{
	X x;
	// perhaps do something to x
	// return std::move(x);  // do not do this!!
	return x; // compiler should do Return Value Optimization
}

void foo1(X&);        // foo1 can be called on l-values.
void foo2(X&&);       // foo2 can be called on r-values.
void foo3(X const &); // foo3 can be called on l-values and r-values.

// temlate argument deduction rule
template<T> void foo4(T &&);
// X A;
// foo4(A);    lvalue, T = X& -> (T&&) = (X& &&).
//             (X& &&) is a reference to a reference. Use reference collapsing rule: (X& &&) -> (X&)
//             result: void foo4(X&);

// foo4(X());  rvalue, T = X  -> (T&&) = (X &&).
//             (X &&) is a rvalue reference to X.
//             result: void foo4(X &&);
int main()
{
	X a, b;
	swap(a, b);
	return 0;
}
