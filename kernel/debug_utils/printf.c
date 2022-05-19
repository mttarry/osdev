#include <kernel/printf.h>
#include <kernel/serial.h>

int is_format_char(char c) {
	if (c == 'd' || c == 'i' || c == 'u' || c == 'o' ||
		c == 'x' || c == 'X' || c == 'f' || c == 'F' ||
		c == 'e' || c == 'E' || c == 'g' || c == 'G' ||
		c == 'a' || c == 'A' || c == 'c' || c == 's' ||
		c == 'p' || c == 'n' || c == '%' )
		return 1;
	else return 0;
}

void vsprintf_helper(char *str, void (*putchar)(char), const char *format, uint32_t *pos, va_list arg) {
	char c;	
	uint32_t uval;
	int32_t ival;
	char buf[512];
	int sign = 0;
	memset(buf, 0, 512);

	while((c = *format++) != 0x0) {
		sign = 0;
		if (c == '%') {
			c = *format++;

			if (!is_format_char(c)) {

			}
			else {

				switch(c) {
					case 'p':
					case 'd':
						uval = ival = va_arg(arg, int);
						if (ival < 0) sign = 1;
						itoa(uval, buf, 10);
						uint32_t len = strlen(buf);

						if (str) {
							for (uint32_t i = 0; i < len; ++i) {
								*(str + *pos) = buf[i];
								*pos = *pos + 1;
							} 
							// TODO
						}
						else {

							char *t = buf;

							if (sign) {
								putchar('-');
							}
							while (*t) {
								putchar(*t++);
							}
						}

						break;


					case 'i':
						break;
					case 'c':

					default:
						break;


				}

				continue;
			}
			

		}
		if (str) {
			*(str + *pos) = c;
			*pos = *pos + 1;
		}
		else {
			(*putchar)(c);
		}


	}

	
}

void vsprintf(char *str, void (*putchar)(char), const char *format, va_list arg) {
	uint32_t pos = 0;
	vsprintf_helper(str, (void*)putchar, format, &pos, arg);
}

void printf(const char *format, ...) {
	va_list args;
	va_start(args, format);
	vsprintf(NULL, (void*)putchar, format, args);
	va_end(args);
}
