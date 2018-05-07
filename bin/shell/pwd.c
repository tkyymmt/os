#include <stdio.h>
#include <file.h>


void pwd()
{
	char buf[256];

	getwd(buf);
	printf("%s\n", buf);
}
