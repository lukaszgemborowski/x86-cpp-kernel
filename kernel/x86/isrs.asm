bits 32

%macro isr 1
global isr%1
extern isr_handler%1
isr%1:
	push eax
	mov eax, [esp + 4]
	pushad
	push eax
	call isr_handler%1
	pop eax
	popad
	pop eax
	iret
%endmacro

%macro isr_code 1
global isr%1
extern isr_handler%1
isr%1:
	push eax
	push ebx
	mov eax, [esp + 8]
	mov ebx, [esp + 12]
	pushad
	push eax
	push ebx
	call isr_handler%1
	pop eax
	popad
	pop ebx
	pop eax
	iret
%endmacro

isr 0
isr 1
isr 2
isr 3
isr 4
isr 5
isr 6
isr 7
isr_code 8
isr 9
isr_code 10
isr_code 11
isr_code 12
isr_code 13
isr_code 14
isr 15
isr 16
isr_code 17
isr 18
isr 19
isr 20

global isr_address_map
isr_address_map:
	dd isr0
	dd isr1
	dd isr2
	dd isr3
	dd isr4
	dd isr5
	dd isr6
	dd isr7
	dd isr8
	dd isr9
	dd isr10
	dd isr11
	dd isr12
	dd isr13
	dd isr14
	dd isr15
	dd isr16
	dd isr17
	dd isr18
	dd isr19
	dd isr20
