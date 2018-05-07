#ifndef SYSCALL_H
#define SYSCALL_H


enum syscall_nr {
	SYS_EXEC,
	SYS_EXIT,
	SYS_PUTS,
	SYS_GETS,
	SYS_GETWD,
	SYS_CHDIR,
	SYS_MKDIR,
	SYS_RMDIR,
	SYS_LIST,
};


__attribute__((always_inline))
inline void exec(int argc, char *argv[])
{
	asm volatile("int	$0x80" :: "b" (SYS_EXEC), "a" (argc), "d" (argv));
}

__attribute__((always_inline))
inline void exit()
{
	asm volatile("int	$0x80" :: "b" (SYS_EXIT));
}


#endif // SYSCALL_H
