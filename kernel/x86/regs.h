#ifndef X86_REGS_H
#define X86_REGS_H

#include <cstdint>

namespace x86
{
namespace regs
{

using reg32 = std::uint32_t;

struct esp {};
struct cr0 {};
struct cr3 {};
struct cr4 {};

template<typename T> reg32 get();

template<> reg32 get<esp>()
{
	reg32 res;
	asm("mov %%esp, %0":"=r"(res));
	return res;
}

template<> reg32 get<cr0>()
{
	reg32 res;
	asm("mov %%cr0, %0":"=r"(res));
	return res;
}

template<> reg32 get<cr3>()
{
	reg32 res;
	asm("mov %%cr3, %0":"=r"(res));
	return res;
}

template<> reg32 get<cr4>()
{
	reg32 res;
	asm("mov %%cr4, %0":"=r"(res));
	return res;
}


std::uint64_t get_msr(std::uint32_t index)
{
	std::uint32_t l, h;

	asm volatile(
		"mov %2, %%ecx\n"
		"rdmsr\n"
		"mov %%eax, %0\n"
		"mov %%edx, %1\n"

		:"=m"(l), "=m"(h)
		:"m"(index)
		:"ecx", "edx", "eax"
	);

	return ((std::uint64_t)h << 32 | l);
}

}
}

#endif
