#ifndef GLXWINDOW_H
#define GLXWINDOW_H


#define MouseButton1	1
#define MouseButton2	2
#define MouseButton3	3

typedef struct _wndparam {
	int	x;
	int	y;
	unsigned int width;
	unsigned int height;
	unsigned int border_width;
} GLXWindowParams;

typedef struct _mouse_event_t {
	unsigned int button;
	int x;
	int y;
} mouse_event_t;


int startGLXWindow(GLXWindowParams *wparam);
void stopGLXWindow(void);

#endif // GLXWINDOW_H
