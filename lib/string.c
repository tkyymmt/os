#include <string.h>


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

void strcpy(char *dst, const char *src)
{
	int count;

	count = strlen(src) + 1;

	for (int i = 0; i < count; ++i)
		dst[i] = src[i];
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
