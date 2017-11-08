#ifndef PRINT_H
#define PRINT_H

#include <type_traits>
#include <limits>
#include <utility>

namespace kernel
{

namespace detail
{

template<typename Out>
void print_one(Out &out, const char *arg)
{
	out.puts(arg);
}

template<typename Int>
struct hex_value
{
	hex_value(Int value) :
		value (value)
	{}

	Int value;
};

template<typename Out, typename Int>
void print_one(Out &out, hex_value<Int> &&value)
{
	constexpr auto buffer_len = sizeof(Int)*2+1;
	char res[buffer_len];

	res[buffer_len - 1] = 0;

	for (auto i = 0; i < sizeof(Int)*2; i ++) {
		const auto curr = (sizeof(Int) * 2) - i - 1;
		res[i] = '0' + ((value.value >> (curr * 4)) & 0xf);
	}

	out.puts("0x");
	out.puts(res);
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

template<typename Int>
auto hex(Int value)
{
	return detail::hex_value<Int>(value);
}

template<typename Out> void print(Out &) {}

template<typename Out, typename First, typename... Args>
void print(Out &dev, First first, Args&&... args)
{
	detail::print_one(dev, std::forward<First>(first));
	print(dev, std::forward<Args>(args)...);
}

}

#endif
