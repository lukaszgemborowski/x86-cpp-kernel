#include "x86/gdt.h"
#include "x86/vga.h"
#include <initializer_list>

std::uint64_t gdt[5];

void x86_init_gdt()
{
	x86::set_gdt_entry(gdt[0], 0, 0, 0);

	int idx = 1;
	for (auto el : {GDT_CODE_PL0, GDT_DATA_PL0, GDT_CODE_PL3, GDT_DATA_PL3}) {
		x86::set_gdt_entry(gdt[idx++], 0, 0xffffffff, el);
	}

	x86::reload_gdt<0x8, 0x10>(x86::gdtr(gdt, 5));
}

extern "C"
void main()
{
	x86::vga screen;
	screen.clear();
	screen.puts("Hello World");

	x86_init_gdt();

	screen.putsat(0, 1, "GDT loaded");
}