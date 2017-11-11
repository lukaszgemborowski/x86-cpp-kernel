#include "x86/gdt.h"
#include "x86/idt.h"
#include "x86/vga.h"
#include "x86/regs.h"
#include <initializer_list>
#include "print.h"

std::uint64_t gdt[5];
kernel::x86::idt_d idt[32];

void x86_init_gdt()
{
	x86::set_gdt_entry(gdt[0], 0, 0, 0);

	int idx = 1;
	for (auto el : {GDT_CODE_PL0, GDT_DATA_PL0, GDT_CODE_PL3, GDT_DATA_PL3}) {
		x86::set_gdt_entry(gdt[idx++], 0, 0xffffffff, el);
	}

	x86::reload_gdt<0x8, 0x10>(x86::gdtr(gdt, 5));
}

x86::vga screen;

void isr()
{
	kernel::println(screen, "interrupt");
	while (true)
		;
}

extern "C"
void main()
{
	screen.clear();
	kernel::println(screen, "Hello: ", 666, ", ", kernel::hex('A'));
	kernel::println(screen, "ESP: ", kernel::hex(x86::regs::get<x86::regs::esp>()));
	kernel::println(screen, "CR0: ", kernel::hex(x86::regs::get<x86::regs::cr0>()));
	kernel::println(screen, "CR4: ", kernel::hex(x86::regs::get<x86::regs::cr4>()));

	x86_init_gdt();

	for (int i = 0; i < 32; i ++) {
		kernel::x86::set_idt_interrupt(idt[i], 8, (std::uint32_t)&isr, true);
	}

	kernel::x86::reload_idt(kernel::x86::idtr(idt, 32));

	kernel::println(screen, "new GDT loaded");
}
