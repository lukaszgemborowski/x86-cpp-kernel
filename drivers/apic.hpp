#ifndef DRIVERS_APIC_HPP
#define DRIVERS_APIC_HPP

#include "kernel/reg_space_def.hpp"
#include "kernel/x86/regs.h"

namespace drivers
{

struct apic
{
	enum class regs : std::uint32_t
	{
		ID = 0x20,
		Version = 0x30
	};

private:
	using apic_space_def = kernel::reg_space_def<
		regs,
		std::uint32_t
	>;

	using sd = apic_space_def;
	static constexpr auto MSR = 0x1b;

public:
	sd::regs<
		sd::r<regs::ID>,
		sd::r<regs::Version>
	> access;

	apic() :
		access (0xFEE00000)
	{}

	bool is_enabled() const
	{
		return x86::regs::get_msr(MSR) & (1 << 11);
	}

	bool is_bootstrap_cpu() const
	{
		return x86::regs::get_msr(MSR) & (1 << 8);
	}
};


} // namespace drivers

#endif
