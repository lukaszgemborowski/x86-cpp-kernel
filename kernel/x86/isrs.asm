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

isr 0
isr 8
