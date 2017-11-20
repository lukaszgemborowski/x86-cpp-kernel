#ifndef KERNEL_X86_PORTS_HPP
#define KERNEL_X86_PORTS_HPP

namespace kernel
{
namespace x86
{
namespace ports
{

template<std::uint8_t N>
void out(std::uint8_t value)
{
	asm volatile(
		"outb %1, %0"
		::"i"(N), "r"(value));
}

template<std::uint8_t N>
std::uint8_t in()
{
	std::uint8_t ret;
	asm volatile(
		"inb %0, %1"
		::"i"(N), "r"(ret));

	return ret;
}

}
}
}

#endif
