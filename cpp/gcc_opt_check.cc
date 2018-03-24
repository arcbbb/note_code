
// to disable optimization for a range of code
#pragma GCC push_options
#pragma GCC optimize (O0)

//your code

#pragma GCC pop_options


// to disable optimization for a single function
void __attribute__((optimize("O0"))) foo(unsigned char data) {
	    // your code
}

int main()
{
}
