#include "x86/gdt.h"
#include "x86/idt.h"
#include "x86/vga.h"
#include "x86/regs.h"
#include "x86/isrs.h"
#include "drivers/apic.hpp"
#include "drivers/pic.hpp"
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

#define HANDLER(n) \
	extern "C" void isr_handler ## n (std::uint32_t eip)          \
	{                                                             \
		kernel::println(screen, "exception ", n,                  \
			", eip: ", kernel::hex(eip));                         \
		asm("hlt");                                               \
	}

#define HANDLER_CODE(n) \
	extern "C" void isr_handler ## n (std::uint32_t code, std::uint32_t eip)          \
	{                                                                                 \
		kernel::println(screen, "exception ", n,                                      \
			", eip: ", kernel::hex(eip), ", code: ", kernel::hex(code));              \
		asm("hlt");                                                                   \
	}

HANDLER(0);
HANDLER(1);
HANDLER(2);
HANDLER(3);
HANDLER(4);
HANDLER(5);
HANDLER(6);
HANDLER(7);
HANDLER_CODE(8);
HANDLER(9);
HANDLER_CODE(10);
HANDLER_CODE(11);
HANDLER_CODE(12);
HANDLER_CODE(13);
HANDLER_CODE(14);
HANDLER(15);
HANDLER(16);
HANDLER_CODE(17);
HANDLER(18);
HANDLER(19);
HANDLER(20);

extern std::uint32_t isr_address_map;

void x86_init_idt()
{
	for (int i = 0; i < 32; i ++) {
		idt.set_interrupt(i, 8, 0, false);
	}

	for (int i = 0; i < 20; i ++) {
		idt.set_interrupt(
			i, 8, static_cast<std::uint32_t>(*(&isr_address_map + i)), true);
	}

	kernel::println(screen, "enabling interrupts");
	idt.reload();
}

extern "C"
void main()
{
	drivers::apic apic;
	drivers::pic pic;
	screen.clear();
	kernel::println(screen, "Hello: ", 666, ", ", kernel::hex('A'));
	kernel::println(screen, "ESP: ", kernel::hex(x86::regs::get<x86::regs::esp>()));
	kernel::println(screen, "CR0: ", kernel::hex(x86::regs::get<x86::regs::cr0>()));
	kernel::println(screen, "CR4: ", kernel::hex(x86::regs::get<x86::regs::cr4>()));
	kernel::println(screen, "APIC MSR: ", kernel::hex(x86::regs::get_msr(0x1b)));
	kernel::println(screen, "APIC enabled: ", apic.is_enabled());
	kernel::println(screen, "APIC version register: 0x", kernel::hex(apic.access.read<drivers::apic::regs::Version>()));

	x86_init_gdt();

	kernel::println(screen, "new GDT loaded\ninitializing IDT...");

	pic.remap_default();
	x86_init_idt();

	// interrupt check
	kernel::println(screen, "Triggering div by zero exception by executing 1/0 statement");

	asm volatile(
		"movw $0, %ax\n"
		"divw %ax, %ax\n");
}
