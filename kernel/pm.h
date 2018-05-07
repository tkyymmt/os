#ifndef PM_H
#define PM_H


#include <stdint.h>

#include "syscall.h"


struct process {
	struct process *parent;
	struct process *child;
	char name[256];
	uint8_t pf_nrs[128];
	struct registers regs;
};


struct process *curr_proc;


extern void create_proc(const char *name);
extern void remove_proc();
extern void init_pm();


#endif // PM_H
