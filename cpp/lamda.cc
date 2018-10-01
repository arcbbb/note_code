
#if 0
[a,&b] where a is captured by copy and b is captured by reference.
[this] captures the current object (*this) by reference
[&] captures all automatic variables used in the body of the lambda by reference and current object by reference if exists
[=] captures all automatic variables used in the body of the lambda by copy and current object by reference if exists
[] captures nothing


[ captures ] <tparams>(optional)(c++20) ( params ) specifiers exception attr -> ret requires(optional)(c++20) { body } (1)
[ captures ] ( params ) -> ret { body }(2)
[ captures ] ( params ) { body } (3)
[ captures ] { body } (4)
#endif

