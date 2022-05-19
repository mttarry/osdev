#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <kernel/printf.h>

#define PORT_COM1 0x3f8 // COM1


void outb(uint16_t port, uint8_t val);
unsigned char inb(uint16_t port);

int init_serial();
int serial_received();
char read_serial();
int is_transmit_empty();
void write_serial(char a);
void qemu_printf(const char * s, ...);

