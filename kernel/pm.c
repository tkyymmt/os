#include <stdint.h>

#include "stdlib.h"
#include "string.h"
#include "pm.h"
#include "fs.h"
#include "asm.h"


void create_proc(const char *name)
{
	curr_proc->child = (struct process *)malloc(sizeof(struct process));
	curr_proc->child->parent = curr_proc;
	curr_proc = curr_proc->child;

	strcpy(curr_proc->name, name);
}

void remove_proc()
{
	curr_proc = curr_proc->parent;

	free(curr_proc->child);
}

void init_pm()
{
	curr_proc = (struct process *)malloc(sizeof(struct process));
	create_proc("shell");

	sti();
	load_file("/bin/shell", 0x10000);
	cli();
}
