#include <syscall.h>
#include <string.h>
#include <file.h>


void main(int argc, char *argv[])
{
	char path[256] = {0};

	if (argc < 2)
		goto ex;

	if (*argv[1] != '/') {
		getwd(path);
		if (strcmp(path, "/") != 0)
			strcat(path, "/");
	}

	strcat(path, argv[1]);
	rmdir(path);

ex:
	exit();
}
