ENTRY(_start)

SECTIONS {
	. = 1M;

	.text BLOCK(4k) : ALIGN(4k) {
		*(.multiboot)
		*(.text)
	}

	.data BLOCK(4k) : ALIGN(4k) {
		*(.data)
	}

	.rodata BLOCK(4k) : ALIGN(4k) {
		*(.rodata)
	}

	.stack BLOCK(4k) : ALIGN(4k) {
		. += 0x4000 ;
		_stack_top = . ;
	}

	.init_array : {
		_init_array_begin = . ;
		*(.init_array)
		_init_array_end = . ;
	}
} 
