#ifndef KERNEL_X86_IDT_H
#define KERNEL_x86_IDT_H

#include <cstdint>

namespace kernel
{
namespace x86
{

using idt_d = std::uint64_t;

void set_idt_interrupt(idt_d &descriptor, std::uint16_t segment, std::uint32_t offset, bool present)
{
	descriptor = (offset >> 16) & 0xffff;
	descriptor <<= 16;
	descriptor |= (present << 15) | 0xE00;
	descriptor <<= 16;
	descriptor |= segment;
	descriptor <<= 16;
	descriptor |= offset & 0xffff;
}

struct __attribute__ ((packed)) idtr {
	std::uint16_t limit;
	std::uint32_t base;

	idtr(std::uint64_t *idt_table, std::size_t count) :
		limit (count * 8 - 1),
		base (reinterpret_cast<std::uint32_t>(idt_table))
	{}
};

void reload_idt(idtr reg)
{
	asm (
		"lidt %0\n"
		"sti\n"
		:: "m"(reg));
}



}
}

#endif
