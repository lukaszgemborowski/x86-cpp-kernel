#ifndef KERNEL_UTILS_BITOPS_HPP
#define KERNEL_UTILS_BITOPS_HPP

#include <meta/meta.hpp>

namespace kernel
{
namespace utils
{

template<typename T>
constexpr T shift(T value, std::size_t N)
{
	return value << N;
}

template<typename T>
constexpr T mask(int N, int S = 0)
{
	return N > 0 ? ((1 << (N - 1)) | mask<T>(N - 1)) << S : 1;
}

template<typename T>
constexpr T binary_or(T a, T b)
{
	return a | b;
}

template<typename Head, typename... Tail>
constexpr Head binary_or(Head head, Tail... tail)
{
	return binary_or(tail...) | head;
}

template<typename T>
constexpr T binary_and(T a, T b)
{
	return a & b;
}

template<typename Head, typename... Tail>
constexpr Head binary_and(Head head, Tail... tail)
{
	binary_and(tail...) & head;
}

template<typename IndexType>
struct with_index
{
	template<IndexType Index, std::size_t a, std::size_t b>
	struct field
	{
		using index_type = IndexType;
		using index = std::integral_constant<IndexType, Index>;
		using left = std::integral_constant<std::size_t, a>;
		using right = std::integral_constant<std::size_t, b>;
	};

	template<typename T, typename... Fields>
	struct bitfield
	{
		using fields = meta::list<Fields...>;

		template<IndexType Index>
		struct match {
			template<typename Arg>
			using invoke = std::integral_constant<bool, Index == Arg::index::value>;
		};

		template<IndexType Index>
		using type_by_index = meta::front<meta::find_if<fields, match<Index>>>;

		template<IndexType Index>
		static T get(const T& value)
		{
			using type = type_by_index<Index>;
			return (mask<T>(type::a::value - type::b::value + 1, type::b::value) & value) >> type::b::value;
		}

		template<IndexType Index>
		static void set(T& value, const T& e)
		{
			using type = type_by_index<Index>;
			value = value | (mask<T>(type::left::value - type::right::value + 1) & e) << type::right::value;
		}
	};

};


}
}

#endif  // KERNEL_UTILS_BITOPS_HPP
