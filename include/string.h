#ifndef STRING_H
#define STRING_H


#include <stddef.h>


extern char *strtok(char *str, const char *delim);
extern int strlen(const char *str);
extern void strcpy(char *dst, const char *src);
extern int strcmp(const char *str1, const char *str2);
extern void strcat(char *str1, const char *str2);


#endif // STRING_H
