#include <syscall.h>
#include <string.h>
#include <file.h>


void main(int argc, char *argv[])
{
	char path[256] = {0};

	switch (argc) {
	case 1:
		getwd(path);
		break;
	case 2:
		if (strcmp(argv[1], ".") == 0) {
			getwd(path);
			break;
		}

		if (strcmp(argv[1], "..") == 0) {
			getwd(path);

			if (strcmp(path, "/") == 0)
				break;

			for (int i = strlen(path) - 1; i; i--) {
				if (path[i] == '/') {
					path[i] = '\0';
					break;
				}
				path[i] = '\0';
			}

			break;
		}

		if (*argv[1] != '/') {
			getwd(path);
			if (strcmp(path, "/") != 0)
				strcat(path, "/");
		}
		
		strcat(path, argv[1]);
		break;
	default:
		goto ex;
	}

	list(path);

ex:
	exit();
}
