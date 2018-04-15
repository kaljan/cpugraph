#ifndef GLXWINDOW_H
#define GLXWINDOW_H


/*!
 * \brief display
 * \note
 * Для работы с OpenGL
 * нам нужно несколько обработчиков:
 * - MouseMoveEvent(x,y)
 * - MousePressEvent(button)
 * - MouseReleaseEvent(button)
 * - MouseWheelEvent(wheel)
 * - KeyPressEvent(keycode)
 * - KeyReleaseEvent(keycode)
 */


typedef struct _wndparam {
	int	x;
	int	y;
	unsigned int width;
	unsigned int height;
	unsigned int border_width;
} GLXWindowParams;


int startGLXWindow(GLXWindowParams *wparam);
void stopGLXWindow(void);

#endif // GLXWINDOW_H
