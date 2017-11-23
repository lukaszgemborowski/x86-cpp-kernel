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
isr 8
isr_code 13
isr_code 14
