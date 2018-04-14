#ifndef GLXWINDOW_H
#define GLXWINDOW_H


typedef struct _wndparam {
	int	x;
	int	y;
	unsigned int width;
	unsigned int height;
	unsigned int border_width;
} wndparam;


int startGLWindow(wndparam *wparam);

#endif // GLXWINDOW_H
