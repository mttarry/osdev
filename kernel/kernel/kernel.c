/* INCLUDES */
#include <stdio.h>
#include <kernel/paging.h>
#include <kernel/tty.h>
#include <kernel/serial.h>
#include <kernel/pfa.h>



void kernel_main() {
	terminal_initialize();

	qemu_printf("Initializing COM1 serial port...\n");
	init_serial();

	qemu_printf("Initializing page frame allocator...\n");
	pfa_init(0x400000);

	paging_init();
}
