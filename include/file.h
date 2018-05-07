#ifndef FILE_H
#define FILE_H


#include <syscall.h>


__attribute__((always_inline))
inline void getwd(char *buf)
{
	asm volatile("int	$0x80" :: "b" (SYS_GETWD), "a" (buf));
}

__attribute__((always_inline))
inline void chdir(const char *path)
{
	asm volatile("int	$0x80" :: "b" (SYS_CHDIR), "a" (path));
}

__attribute__((always_inline))
inline void mkdir(const char *path)
{
	asm volatile("int	$0x80" :: "b" (SYS_MKDIR), "a" (path));
}

__attribute__((always_inline))
inline void rmdir(const char *path)
{
	asm volatile("int	$0x80" :: "b" (SYS_RMDIR), "a" (path));
}

__attribute__((always_inline))
inline void list(const char *path)
{
	asm volatile("int	$0x80" :: "b" (SYS_LIST), "a" (path));
}


#endif // FILE_H
