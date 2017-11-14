#ifndef KERNEL_X86_IDT_H
#define KERNEL_x86_IDT_H

#include <cstdint>

namespace kernel
{
namespace x86
{

template<std::size_t Size>
class idt_table
{
	using idt_d = std::uint64_t;

	/* IDT register is 48 bit long, first 16 bit value is
	 * so called "limit", holding an offset to last byte
	 * in IDT table. Next theere are 32 bits of base address
	 * this is actual pointer to IDT table. This is just a helper
	 * class to store data properly. */
	struct __attribute__ ((packed)) idtr {
		std::uint16_t limit;
		std::uint32_t base;

		idtr(std::uint64_t *idt_table, std::size_t count) :
			limit (count * 8 - 1),
			base (reinterpret_cast<std::uint32_t>(idt_table))
		{}
	};

	/* Load IDR register, this makes new IDT table "active" */
	void reload_idt(idtr reg)
	{
		asm volatile (
			"lidt %0\n"
			"sti\n"
			:: "m"(reg));
	}

public:
	void set_interrupt(std::size_t n, std::uint16_t segment, std::uint32_t offset, bool present)
	{
		auto &descriptor = idt[n];

		descriptor = (offset >> 16) & 0xffff;
		descriptor <<= 16;
		descriptor |= (present << 15) | 0xE00;
		descriptor <<= 16;
		descriptor |= segment;
		descriptor <<= 16;
		descriptor |= offset & 0xffff;
	}

	void reload()
	{
		idtr r(idt, Size);
		reload_idt(r);
	}

private:
	idt_d idt[Size];
};

} // namespace x86
} // namespace kernel

#endif
