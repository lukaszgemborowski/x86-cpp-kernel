#ifndef DRIVERS_PIT_HPP
#define DRIVERS_PIT_HPP

#include <cstdint>
#include "kernel/reg_space_def.hpp"

namespace drivers
{

struct pit
{
	enum class regs : std::uint8_t
	{
		Channel0 = 0x40,
		Channel1 = 0x41,
		Channel2 = 0x42,
		Command = 0x43
	};

private:
	using sd = kernel::reg_space_def<
		regs,
		std::uint8_t,
		kernel::PortsAccessor
	>;

public:
	sd::regs<
		sd::r<regs::Channel0>,
		sd::r<regs::Channel1>,
		sd::r<regs::Channel2>,
		sd::r<regs::Command>
	> access;

	pit() : access(0) {}
};

} // namespace drivers

#endif
