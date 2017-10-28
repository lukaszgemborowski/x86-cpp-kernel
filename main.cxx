#include "x86/gdt.h"

std::uint64_t gdt[5];

void putchar(int x, int y, char chr)
{
	constexpr auto width = 80;
	unsigned char *vga = reinterpret_cast<unsigned char *>(0xB8000);

	*(vga + 2*(y * width + x)) = chr;
}

void putstring(int x, int y, const char *text)
{
	while (*text) {
		putchar(x++, y, *text);
		text ++;
	}
}

void x86_init_gdt()
{
	x86::set_gdt_entry(gdt[0], 0, 0, 0);
	x86::set_gdt_entry(gdt[1], 0, 0xffffffff, GDT_CODE_PL0);
	x86::set_gdt_entry(gdt[2], 0, 0xffffffff, GDT_DATA_PL0);
	x86::set_gdt_entry(gdt[3], 0, 0xffffffff, GDT_CODE_PL3);
	x86::set_gdt_entry(gdt[4], 0, 0xffffffff, GDT_DATA_PL3);
	x86::reload_gdt<0x8, 0x10>(x86::gdtr(gdt, 5));
}

extern "C"
void main()
{
	putchar(0, 0, 'X');
	putstring(0, 0, "Hello World");

	x86_init_gdt();

	putstring(0, 1, "GDT loaded");
}
