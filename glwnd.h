#ifndef GLWND_H
#define GLWND_H


typedef struct _wndparam {
	int	x;
	int	y;
	unsigned int width;
	unsigned int height;
	unsigned int border_width;
} wndparam;


int startGLWindow(wndparam *wparam);

#endif // GLWND_H
