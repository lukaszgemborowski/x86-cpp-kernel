#ifndef KERNEL_REG_SPACE_DEF_HPP
#define KERNEL_REG_SPACE_DEF_HPP

#include <meta/meta.hpp>

namespace kernel
{

template<typename N, typename T>
struct reg_space_def
{
	template<N n>
	struct r
	{
		using name = std::integral_constant<N, n>;
		using offset = std::integral_constant<
			std::uint32_t,
			static_cast<typename std::underlying_type<N>::type>(n)
			>;
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
		std::uint32_t address() const
		{
			using find_register = typename meta::find_if<regs_list, reg_match<R>>::type;
			using reg_type = meta::first<find_register>;

			return base_ + reg_type::offset::value;
		}

		template<N R>
		T read()
		{
			return *(T *)(address<R>());
		}

		std::uint32_t base_;
	};
};

} // namespace kernel

#endif
