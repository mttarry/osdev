#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>


int is_format_char(char c);
void vsprintf_helper(char *str, void (*putchar)(char), const char *format, uint32_t *pos, va_list arg);
void vsprintf(char *str, void (*putchar)(char), const char *format, va_list arg);
void printf(const char *format, ...);