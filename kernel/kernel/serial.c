#include <kernel/serial.h>

void outb(uint16_t port, uint8_t val) {
    asm volatile("outb %1, %0" : : "dN"(port), "a"(val));
}

unsigned char inb(uint16_t port) {
    char val;
    asm volatile("inb %1, %0" : "=a" (val) : "dN" (port));
    return val;
}

int init_serial() {
    outb(PORT_COM1 + 1, 0x00);    // Disable all interrupts
    outb(PORT_COM1 + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(PORT_COM1 + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outb(PORT_COM1 + 1, 0x00);    //                  (hi byte)
    outb(PORT_COM1 + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(PORT_COM1 + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb(PORT_COM1 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
    outb(PORT_COM1 + 4, 0x1E);    // Set in loopback mode, test the serial chip
    outb(PORT_COM1 + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)
 
    // Check if serial is faulty (i.e: not same byte as sent)
    if(inb(PORT_COM1 + 0) != 0xAE) {
       return 1;
    }
 
    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    outb(PORT_COM1 + 4, 0x0F);
    return 0;
}

int serial_received() {
   return inb(PORT_COM1 + 5) & 1;
}
 
char read_serial() {
   while (serial_received() == 0);
 
   return inb(PORT_COM1);
}

int is_transmit_empty() {
   return inb(PORT_COM1 + 5) & 0x20;
}
 
void write_serial(char a) {
   while (is_transmit_empty() == 0);
 
   outb(PORT_COM1,a);
}

/*
* Print to QEMU's log
 * */
void qemu_printf(const char * s, ...) {
    va_list(ap);
    va_start(ap, s);
    vsprintf(NULL, write_serial, s, ap);
    va_end(ap);
}