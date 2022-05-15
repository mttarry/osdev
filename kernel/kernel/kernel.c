/* INCLUDES */
#include <stdio.h>
#include <kernel/paging.h>
#include <kernel/tty.h>





void kernel_main() {
	terminal_initialize();
	terminal_writestring("FSDFDS");
}
