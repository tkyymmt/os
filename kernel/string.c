#include "string.h"


void memset(void *dst, int value, size_t size)
{
	unsigned char *d = (unsigned char *)dst;

	for (size_t i = 0; i < size; i++)
		d[i] = value;
}

void memcpy(void *dst, const void *src, size_t size)
{
	char *d;
	const char *s;

	d = (char *)dst;
	s = (const char *)src;

	for (size_t i = 0; i < size; ++i)
		d[i] = s[i];
}

void strcpy(char *dst, const char *src)
{
	int count;

	count = strlen(src) + 1;

	memcpy(dst, src, count);
}

char *strtok(char *str, const char *delim)
{
	static char *buf;
	char *tok = NULL;

	if (str != NULL)
		buf = str;

	while (*buf) {
		tok = buf;

		for (int i = 0; delim[i]; ++i) {
			if (*buf == delim[i])
				++buf;
		}

		if (tok == buf)
			break;
	}

	while (*buf) {
		for (int i = 0; delim[i]; ++i) {
			if (*buf == delim[i]) {
				*buf = '\0';
				++buf;
				goto out;
			}
		}

		++buf;
	}

out:
	return tok;
}

int strlen(const char *str)
{
	int len = 0;

	while (str[len])
		++len;

	return len;
}

int strcmp(const char *str1, const char *str2)
{
    int diff = 0;

    for (int i = 0; !diff; ++i) {
		diff = str1[i] - str2[i];
		if (!str1[i] && !str2[i])
			break;
    }

    return diff;
}

void strcat(char *str1, const char *str2)
{
	int idx, len;

	idx = strlen(str1);
	len = strlen(str2);

	for (int i = 0; i <= len; ++i, ++idx)
		str1[idx] = str2[i];
}
