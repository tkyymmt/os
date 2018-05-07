#include <stdarg.h>
#include <stdint.h>

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "syscall.h"
#include "fs.h"
#include "pm.h"
#include "mm.h"
#include "keyboard.h"
#include "display.h"
#include "asm.h"


void *syscalls[] = {
	do_exec,
	do_exit,
	do_puts,
	do_gets,
	do_getwd,
	do_chdir,
	do_mkdir,
	do_rmdir,
	do_list,
};


__attribute__((regparm(2)))
void do_exec(int argc, char *argv[])
{
	struct registers *regs;
	char *args[argc];
	void *ptr;
	char path[5+256];

	regs = (struct registers *)0xffac;

	curr_proc->regs = *regs;
/*
	curr_proc->regs.edi = regs->edi;
	curr_proc->regs.esi = regs->esi;
	curr_proc->regs.ebp = regs->ebp;
	curr_proc->regs.esp = regs->esp;
	curr_proc->regs.ebx = regs->ebx;
	curr_proc->regs.frame.eip = regs->frame.eip;
	curr_proc->regs.frame.esp = regs->frame.esp;
*/

	for (int i = 0; i < argc; ++i) {
		args[i] = (char *)malloc(strlen(argv[i]) + 1);
		strcpy(args[i], argv[i]);
	}

	take_pages(curr_proc->pf_nrs);
	create_proc(args[0]);

	ptr = (void *)0x8fff0;

	for (int i = argc; i;) {
		ptr -= strlen(args[(--i)]) + 1;
		strcpy((char *)ptr, args[i]);
		args[i] = (char *)ptr;
	}

	ptr -= sizeof(char *) * argc;
	memcpy(ptr, args, sizeof(char *) * argc);

	argv = (char **)ptr;

	ptr -= (int)ptr % 4;

	ptr -= sizeof(char **);
	memcpy(ptr, &argv, sizeof(char **));

	ptr -= sizeof(int);
	memcpy(ptr, &argc, sizeof(int));

	regs->frame.eip = 0x10000;
	regs->frame.esp = (uint32_t)ptr - 4; // eip

	strcpy(path, "/bin/");
	strcat(path, args[0]);

	sti();
	load_file(path, 0x10000);
}

void do_exit()
{
	struct registers *regs;

	remove_proc();
	set_pages(curr_proc->pf_nrs);

	regs = (struct registers *)0xffac;

	*regs = curr_proc->regs;
/*
	regs->edi = curr_proc->regs.edi;
	regs->esi = curr_proc->regs.esi;
	regs->ebp = curr_proc->regs.ebp;
	regs->esp = curr_proc->regs.esp;
	regs->ebx = curr_proc->regs.ebx;
	regs->frame.eip = curr_proc->regs.frame.eip;
	regs->frame.esp = curr_proc->regs.frame.esp;
*/
}

__attribute__((regparm(1)))
void do_puts(const char *str)
{
	printf("%s", str);
}

__attribute__((regparm(1)))
void do_gets(char *buf)
{
	clear_key_buf();

	while (1) {
		*buf = read_keyboard();
		write_display(*buf);

		if (*buf == '\n')
			break;
		else if (*buf == '\b')
			buf--;
		else if (*buf)
			buf++;
	}

	*buf = '\0';
}

__attribute__((regparm(1)))
void do_getwd(char *buf)
{
	strcpy(buf, work_dir);
}

__attribute__((regparm(1)))
void do_chdir(const char *path)
{
	sti();
	if (is_directory(path))
		strcpy(work_dir, path);
}

__attribute__((regparm(1)))
void do_mkdir(const char *path)
{
	sti();
	make_dir(path);
}

__attribute__((regparm(1)))
void do_rmdir(const char *path)
{
	sti();
	remove_dir(path);
}

__attribute__((regparm(1)))
void do_list(const char *path)
{
	sti();
	list_df(path);
}
