#include <stdarg.h>

#include "display.h"


static void putint(int value, const int base)
{
	char buf[32] = {'\0'};
	const int spare = value;
	int i = 0;

	do {
		buf[++i] = "fedcba9876543210123456789abcdef"[15 + value % base];
		value /= base;
	} while (value);

	if (spare < 0)
		buf[++i] = '-';

	for (; i; i--)
		write_display(buf[i]);
}

void printf(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);

	while (*fmt) {
		if (*fmt == '%') {
			switch (*(++fmt)) {
			case 'c':
			{
				char c = va_arg(args, int);
				write_display(c);
				break;
			}
			case 'd':
			{
				int d = va_arg(args, int);
				putint(d, 10);
				break;
			}
			case 'x':
			{
				int x = va_arg(args, int);
				putint(x, 16);
				break;
			}
			case 's':
			{
				const char *s = va_arg(args, char *);
				for (int i = 0; s[i]; ++i)
					write_display(s[i]);
				break;
			}
			default:
				break;
			}
			++fmt;
			continue;
		}
		write_display(*fmt);
		++fmt;
	}

	va_end(args);
}
