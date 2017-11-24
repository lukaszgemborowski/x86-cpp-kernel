#ifndef DRIVERS_PIC_HPP
#define DRIVERS_PIC_HPP

#include "kernel/reg_space_def.hpp"

namespace drivers
{

struct pic
{
	enum class regs : std::uint8_t
	{
		Pic1Command = 0x20,
		Pic1Data = 0x21,
		Pic2Command = 0xA0,
		Pic2Data = 0xA1
	};

	using sd = kernel::reg_space_def<
		regs,
		std::uint8_t,
		kernel::PortsAccessor>;

	sd::regs<
		sd::r<regs::Pic1Command>,
		sd::r<regs::Pic1Data>,
		sd::r<regs::Pic2Command>,
		sd::r<regs::Pic2Data>>
	access;

	pic () :
		access (0)
	{
	}

	void remap_default() const
	{
		access.write<regs::Pic1Command>(0x11);
		access.write<regs::Pic2Command>(0x11);

		access.write<regs::Pic1Data>(0x20);
		access.write<regs::Pic2Data>(0x28);

		access.write<regs::Pic1Data>(0x04);
		access.write<regs::Pic2Data>(0x02);

		access.write<regs::Pic1Data>(0x01);
		access.write<regs::Pic2Data>(0x01);
	}
};

} // namespace driers

#endif
