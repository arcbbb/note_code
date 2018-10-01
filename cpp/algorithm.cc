
upper_bound: returns the first element in the range that is greater than value.
first >
lower_bound: returns the first element in the range that is not less than value (>= value).
first >=
(o,o,o,o,x,x,x)
return the first false value

given data = {1,2,3,4,4,4,5}
upper_bound(data.begin(), data.end(), 4) should be 5.       (5 >4)
{1,2,3,4,4,4}
!(value < element)
comp(value, element)

lower_bound(data.begin(), data.end(), 4) should be first 4. (4>=4)
{1,2,3}
element < value
comp(element, value)

comp: binary predicate which returns true if the first argument is less than the second. (is ordered).


void demo_transform()
{
	// char to uppercase, in-place replacement
	std::string s("hello");
	std::transform(s.begin(), s.end(), s.begin(),
			[](unsigned char c) -> unsigned char { return std::toupper(c); });

	// char to ordinal value
	std::vector<std::size_t> ordinals;
	std::transform(s.begin(), s.end(), std::back_inserter(ordinals),
			[](unsigned char c) -> std::size_t { return c; });
}
