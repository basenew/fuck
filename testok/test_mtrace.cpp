#include <stdio.h>
#include <stdlib.h>
#include <mcheck.h>

int main()
{
	setenv("MALLOC_TRACE", "taoge.log", 1);
	mtrace();

	int *p = (int *)new char[2 * sizeof(int)];

	return 0;
}
