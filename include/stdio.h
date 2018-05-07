#ifndef STDIO_H
#define STDIO_H


#include <syscall.h>
#include <stdarg.h>


extern void printf(const char *fmt, ...);

__attribute__((always_inline))
inline void puts(const char *str)
{
	asm volatile("int	$0x80" :: "b" (SYS_PUTS), "a" (str));
}

__attribute__((always_inline))
inline void gets(char *buf)
{
	asm volatile("int	$0x80" :: "b" (SYS_GETS), "a" (buf));
}


#endif // STDIO_H
