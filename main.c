#include "gdefs.h"
#include "glwnd.h"

#include <stdio.h>

int main(int argc, char **argv)
{
	UNUSED(argc);
	UNUSED(argv);

	wndparam wp;
	wp.x = 100;
	wp.y = 100;
	wp.width = 640;
	wp.height = 480;
	wp.border_width = 0;

	return startGLWindow(&wp);
}
