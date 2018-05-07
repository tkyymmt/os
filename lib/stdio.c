#include <stdio.h>
#include <string.h>


static void itoa(int value, char *str, int radix)
{
	char *ptr, *low;
	char tmp;

	ptr = str;

	if (value < 0 && radix == 10)
		*ptr++ = '-';

	low = ptr;

	do {
		*ptr++ = "fedcba9876543210123456789abcdef"[15 + value % radix];
		value /= radix;
	} while (value);

	*ptr-- = '\0';

	while (low < ptr) {
		tmp = *low;
		*low++ = *ptr;
		*ptr-- = tmp;
	}
}

void printf(const char *fmt, ...)
{
	char str[128] = {0}, a[33], *ptr, *s;
	int dx;
	va_list args;

	va_start(args, fmt);

	ptr = str;

	while (*fmt) {
		if (*fmt == '%') {
			switch (*(++fmt)) {
			case 'c':
				*ptr++ = va_arg(args, int);
				break;
			case 'd':
				dx = va_arg(args, int);
				itoa(dx, a, 10);
				*ptr = '\0';
				strcat(str, a);
				ptr += strlen(a);
				break;
			case 'x':
				dx = va_arg(args, int);
				itoa(dx, a, 16);
				*ptr = '\0';
				strcat(str, a);
				ptr += strlen(a);
				break;
			case 's':
				s = va_arg(args, char *);
				*ptr = '\0';
				strcat(str, s);
				ptr += strlen(s);
				break;
			default:
				break;
			}
		} else {
			*ptr++ = *fmt;
		}

		fmt++;
	}

	va_end(args);

	*ptr = '\0';
	puts(str);
}
