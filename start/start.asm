%define MULTIBOOT_HEADER_MAGIC                  0x1BADB002
%define MULTIBOOT_PAGE_ALIGN                    0x00000001
%define MULTIBOOT_MEMORY_INFO                   0x00000002

%define MULTIBOOT_HEADER_FLAGS                  MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO

bits 32

global _start
extern main, _stack_top, init_globals

section .multiboot
	align 4
	dd MULTIBOOT_HEADER_MAGIC
	dd MULTIBOOT_HEADER_FLAGS

; TODO: calculate at compile time, -(magic + flags) gives wrong answer
	dd 0xE4524FFB

section .text
_start:
	mov esp, _stack_top
	call init_globals
	call main
	cli
	hlt

dead:
	jmp dead
