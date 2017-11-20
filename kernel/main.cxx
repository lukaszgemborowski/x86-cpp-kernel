#include "x86/gdt.h"
#include "x86/idt.h"
#include "x86/vga.h"
#include "x86/regs.h"
#include "x86/isrs.h"
#include "drivers/apic.hpp"
#include <initializer_list>
#include "print.h"

std::uint64_t gdt[5];
kernel::x86::idt_table<32> idt;

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

extern "C" void isr_handler0(std::uint32_t eip)
{
	kernel::println(screen, "div by zero from: 0x", kernel::hex(eip));
	asm("hlt");
}

extern "C" void isr_handler8(std::uint32_t source_ip)
{
	kernel::println(screen, "double fault from ", kernel::hex(source_ip) ,", aborting");

	asm("hlt");
}

extern "C"
void main()
{
	drivers::apic apic;
	screen.clear();
	kernel::println(screen, "Hello: ", 666, ", ", kernel::hex('A'));
	kernel::println(screen, "ESP: ", kernel::hex(x86::regs::get<x86::regs::esp>()));
	kernel::println(screen, "CR0: ", kernel::hex(x86::regs::get<x86::regs::cr0>()));
	kernel::println(screen, "CR4: ", kernel::hex(x86::regs::get<x86::regs::cr4>()));
	kernel::println(screen, "APIC MSR: ", kernel::hex(x86::regs::get_msr(0x1b)));
	kernel::println(screen, "APIC enabled: ", apic.is_enabled());

	x86_init_gdt();

	for (int i = 0; i < 32; i ++) {
		idt.set_interrupt(i, 8, 0, false);
	}

	idt.set_interrupt(0, 8, (std::uint32_t)&isr0, true);
	idt.set_interrupt(8, 8, (std::uint32_t)&isr8, true);

	idt.reload();

	kernel::println(screen, "new GDT loaded");
	kernel::println(screen, "APIC version register: 0x", kernel::hex(apic.access.read<drivers::apic::regs::Version>()));

	// interrupt check
	kernel::println(screen, "Triggering div by zero exception by executing 1/0 statement");
	asm volatile(
		"movw $0, %ax\n"
		"divw %ax, %ax\n");
}
