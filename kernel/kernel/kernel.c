/* INCLUDES */
#include <stdio.h>
#include <kernel/paging.h>
#include <kernel/tty.h>
#include <kernel/serial.h>
#include <kernel/pfa.h>



void kernel_main() {
	terminal_initialize();
	init_serial();

	
}
