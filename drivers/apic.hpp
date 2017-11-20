#ifndef DRIVERS_APIC_HPP
#define DRIVERS_APIC_HPP

#include "kernel/reg_space_def.hpp"

namespace drivers
{

struct apic
{
	enum class regs : std::uint32_t
	{
		ID = 0x20,
		Version = 0x30
	};

	using apic_space_def = kernel::reg_space_def<
		regs,
		std::uint32_t
	>;

	using sd = apic_space_def;

	sd::regs<
		sd::r<regs::ID>,
		sd::r<regs::Version>
	> access;

	apic() :
		access (0xFEE00000)
	{}
};


} // namespace drivers

#endif
