#ifndef SYSCALL_H
#define SYSCALL_H


#include <stdint.h>
#include <stdarg.h>


struct int_frame {
	uint32_t eip;
	uint16_t cs, :16;
	uint32_t eflags;
	uint32_t esp;
	uint16_t ss, :16;
};

struct registers {
	uint16_t gs, :16;
	uint16_t fs, :16;
	uint16_t es, :16;
	uint16_t ds, :16;
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t esp;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;
	struct int_frame frame;
};


extern void do_exec(int argc, char *argv[]) __attribute__((regparm(2)));
extern void do_exit();
extern void do_puts(const char *str) __attribute__((regparm(1)));
extern void do_gets(char *buf) __attribute__((regparm(1)));
extern void do_getwd(char *buf) __attribute__((regparm(1)));
extern void do_chdir(const char *path) __attribute__((regparm(1)));
extern void do_mkdir(const char *path) __attribute__((regparm(1)));
extern void do_rmdir(const char *path) __attribute__((regparm(1)));
extern void do_list(const char *path) __attribute__((regparm(1)));


#endif // SYSCALL_H
