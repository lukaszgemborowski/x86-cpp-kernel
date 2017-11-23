#ifndef KERNEL_REG_SPACE_DEF_HPP
#define KERNEL_REG_SPACE_DEF_HPP

#include <meta/meta.hpp>
#include <kernel/x86/ports.hpp>

namespace kernel
{

struct MemoryAccessor
{
	template<typename Ret>
	static Ret read(std::size_t address)
	{
		return *reinterpret_cast<Ret *>(address);
	}

	template<typename WriteType>
	static void write(std::size_t address, WriteType data)
	{
		*reinterpret_cast<WriteType *>(address) = data;
	}
};

struct PortsAccessor
{
	template<typename Ret>
	static Ret read(std::size_t address)
	{
		return kernel::x86::ports::in<Ret>(address);
	}

	template<typename WriteType>
	static void write(std::size_t address, WriteType data)
	{
		kernel::x86::ports::out<WriteType>(address, data);
	}
};

template<typename N, typename T, typename Accessor = MemoryAccessor>
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
			return Accessor::template read<RetType>(address<R>());
		}

		template<N R, typename WriteType = typename register_type<R>::type>
		void write(WriteType data) const
		{
			Accessor::template write<WriteType>(address<R>(), data);
		}

	private:
		std::uint32_t base_;
	};
};

} // namespace kernel

#endif
