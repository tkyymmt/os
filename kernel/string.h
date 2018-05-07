#ifndef STRING_H
#define STRING_H


#include <stddef.h>


extern void memset(void *ptr, int value, size_t size);
extern void memcpy(void *dst, const void *src, size_t size);
extern void strcpy(char *dst, const char *src);
extern char *strtok(char *str, const char *delim);
extern int strlen(const char *str);
extern int strcmp(const char *str1, const char *str2);
extern void strcat(char *str1, const char *str2);


#endif // STRING_H
