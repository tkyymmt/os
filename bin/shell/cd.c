#include <file.h>
#include <string.h>


#include <stdio.h>
void cd(int argc, char *argv[])
{
	char path[256] = {0};

	if (argc < 2)
		return;

	if (strcmp(argv[1], ".") == 0)
		return;

	if (strcmp(argv[1], "..") == 0) {
		getwd(path);

		if (strcmp(path, "/") == 0)
			return;

		for (int i = strlen(path) - 1; i; i--) {
			if (path[i] == '/') {
				path[i] = '\0';
				break;
			}
			path[i] = '\0';
		}
	} else {
		if (*argv[1] != '/')
			getwd(path);
		strcat(path, argv[1]);
	}

	chdir(path);
}
