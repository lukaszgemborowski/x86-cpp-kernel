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

}
}

#endif
