#include <syscall.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

#include "builtin.h"


__attribute__((section(".entry")))
void main()
{
	while (1) {
		char cmdline[256];
		char *argv[128];
		int argc = 0;

		puts("# ");
		gets(cmdline);

		argv[argc++] = strtok(cmdline, " ");
		while (argv[argc - 1] != NULL)
			argv[argc++] = strtok(NULL, " ");
		argc--;

		if (strcmp(argv[0], "echo") == 0) {
			echo(argc, argv);
			continue;
		}

		if (strcmp(argv[0], "pwd") == 0) {
			pwd();
			continue;
		}

		if (strcmp(argv[0], "cd") == 0) {
			cd(argc, argv);
			continue;
		}

		exec(argc, argv);
	}
}
