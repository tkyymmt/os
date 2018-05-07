#include <syscall.h>
#include <string.h>
#include <file.h>


void main(int argc, char *argv[])
{
	char path1[256] = {0}, path2[256] = {0};

	if (argc < 3)
		goto ex;

	if (*argv[1] != '/') {
		getwd(path1);
		if (strcmp(path1, "/") != 0)
			strcat(path1, "/");
	}
	if (*argv[2] != '/') {
		getwd(path2);
		if (strcmp(path2, "/") != 0)
			strcat(path2, "/");
	}

	strcat(path1, argv[1]);
	strcat(path2, argv[2]);

	rmdir(path1);
	mkdir(path2);

ex:
	exit();
}
