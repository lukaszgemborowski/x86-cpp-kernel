#ifndef KERNEL_REG_SPACE_DEF_HPP
#define KERNEL_REG_SPACE_DEF_HPP

#include <meta/meta.hpp>

namespace kernel
{

template<typename N, typename T>
struct reg_space_def
{
	template<N n, typename RT = T>
	struct r
	{
		using name = std::integral_constant<N, n>;
		using offset = std::integral_constant<
			std::uint32_t,
			static_cast<typename std::underlying_type<N>::type>(n)
			>;
		using type = RT;
	};

	template<typename... Regs>
	struct regs
	{
		regs(std::uint32_t base) :
			base_(base)
		{}

		using regs_list = meta::list<Regs...>;

		template<N n>
		struct reg_match {
			template<typename R> using invoke =
				std::integral_constant<bool, R::name::value == n>;
		};

		template<N R>
		using find_register_sublist = typename meta::find_if<regs_list, reg_match<R>>::type;

		template<N R>
		using register_type = meta::first<find_register_sublist<R>>;

		template<N R>
		std::uint32_t address() const
		{
			return base_ + register_type<R>::offset::value;
		}

		template<N R, typename RetType = typename register_type<R>::type>
		RetType read() const
		{
			return *reinterpret_cast<RetType *>(address<R>());
		}

		std::uint32_t base_;
	};
};

} // namespace kernel

#endif
