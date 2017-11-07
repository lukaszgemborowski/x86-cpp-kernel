#ifndef PRINT_H
#define PRINT_H

#include <type_traits>
#include <limits>

namespace kernel
{

namespace detail
{

template<typename Out>
void print_one(Out &out, const char *arg)
{
	out.puts(arg);
}

template<typename Out, typename Int>
std::enable_if_t<std::is_integral<std::remove_reference_t<Int>>::value> print_one(Out &out, Int&& value)
{
	using IntType = std::remove_reference<Int>;
	constexpr auto buffer_len = std::numeric_limits<IntType>::digits10 + 3;

	char res[buffer_len];
	auto negative = false;
	res[buffer_len - 1] = 0;

	if (value < 0) {
		negative = true;
	}

	auto pos = buffer_len - 2;
	while (value > 0) {
		int part = value % 10;
		value = value / 10;
		res[pos--] = '0' + part;
	}

	if (negative) {
		res[pos--] = '-';
	}

	out.puts(&res[pos + 1]);
}

} // detail

template<typename Out> void print(Out &) {}

template<typename Out, typename First, typename... Args>
void print(Out &dev, First first, Args&&... args)
{
	detail::print_one(dev, first);
	print(dev, args...);
}

}

#endif
