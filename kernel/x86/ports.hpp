#ifndef KERNEL_X86_PORTS_HPP
#define KERNEL_X86_PORTS_HPP

namespace kernel
{
namespace x86
{
namespace ports
{

template<typename T, std::uint32_t N>
void out(T value)
{
	asm volatile(
		"out %1, %0"
		::"i"(N), "r"(value));
}

template<typename T, std::uint32_t N>
T in()
{
	T ret;
	asm volatile(
		"in %0, %1"
		::"i"(N), "r"(ret));

	return ret;
}

}
}
}

#endif
