#include <memory>

int main()
{
	int sz = 1024;
	// std::shared_ptr<int> shared_bad(new int[10]);
	// the destructor calls delete, undefined behavior
	std::shared_ptr<int> sbuf(new int[sz], std::default_delete<int[]>());
	// the destructor calls delete[], ok

	std::unique_ptr<int[]> ubuf(new int[sz]);
	// unique_ptr<int[]> uses default_delete<int[]>

	// for void
	using deleter_t = std::function<void(void *)>;
	std::unique_ptr<void, deleter_t> uniq_void_ptr;

	int count = 1024;
	std::unique_ptr<float[]> input_;
	input_ = {new float[count], std::default_delete<float[]>()};
	return 0;
}
