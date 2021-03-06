#include <stdlib.h>
#include <string.h>
#include <kernel/serial.h>

void reverse(char str[], int length)
{
	for (int i = 0, j = length - 1; i < length / 2; ++i, --j) {
		char tmp = str[i];
		str[i] = str[j];
		str[j] = tmp;
	}   
}

void itoa(int num, char* str, int base)
{
    int i = 0;
    int isNegative = 0;
 
    /* Handle 0 explicitly, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
    }
 
    // In standard itoa(), negative numbers are handled only with
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = 0;
        num = -num;
    }
 
    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        if (base == 16 && rem < 0) rem = -rem;
        str[i++] = (rem > 9) ? (rem-10) + 'a' : rem + '0';
        num = num / base;
    }
 
    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';
 
    str[i] = '\0'; // Append string terminator
 
    // Reverse the string
    reverse(str, i);
 }

 void utoa(uint32_t num, char* str, int base)
{
    int i = 0;
 
    /* Handle 0 explicitly, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
    }

    // Process individual digits
    while (num != 0)
    {
        uint32_t rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }
 
    str[i] = '\0'; // Append string terminator

    // Reverse the string
    reverse(str, i);

}
