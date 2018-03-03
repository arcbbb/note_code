#include <iostream>
using namespace std;

class Widget {
	public:
		Widget();                             // default constructor
		Widget(const Widget& rhs);            // copy constructor
		Widget& operator=(const Widget& rhs); // copy assignment operator
		bool status;
};

bool test_Widget(Widget w);

int main()
{
	Widget w1;       // default constructor
	Widget w2(w1);   // copy constructor
	Widget w3 = w2;  // copy constructor

	cout << "bool= " << test_Widget(w2) << endl; // copy constructor
	w1 = w2;         // copy assignment operator
	return 0;
}

Widget::Widget()
{
	cout << "default constructor" << endl;
	status = false;
}

Widget::Widget(const Widget& rhs)
{
	cout << "copy constructor" << endl;
}

Widget& Widget::operator=(const Widget& rhs)
{
	cout << "copy assignment operator" << endl;
}

bool test_Widget(Widget w)
{
	return w.status;
}
