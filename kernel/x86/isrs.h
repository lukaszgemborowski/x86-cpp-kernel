#ifndef KERNEL_X86_ISRS_H
#define KERNEL_X86_ISRS_H

#define EXTERN_ISR(N) \
	extern "C" void isr ## N (); \
	extern "C" void isr_handler ## N (std::uint32_t eip);

#define EXTERN_ISR_CODE(N) \
	extern "C" void isr ## N (); \
	extern "C" void isr_handler ## N (std::uint32_t code, std::uint32_t eip);

EXTERN_ISR(0);
EXTERN_ISR(1);
EXTERN_ISR(2);
EXTERN_ISR(3);
EXTERN_ISR(4);
EXTERN_ISR(5);
EXTERN_ISR(6);
EXTERN_ISR(7);
EXTERN_ISR_CODE(8);
EXTERN_ISR(9);
EXTERN_ISR_CODE(10);
EXTERN_ISR_CODE(11);
EXTERN_ISR_CODE(12);
EXTERN_ISR_CODE(13);
EXTERN_ISR_CODE(14);
EXTERN_ISR(15);
EXTERN_ISR(16);
EXTERN_ISR_CODE(17);
EXTERN_ISR(18);
EXTERN_ISR(19);
EXTERN_ISR(20);

#endif
