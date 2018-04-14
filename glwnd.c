#include "gdefs.h"
#include "glwnd.h"
#include "eh.h"

#include <stdio.h>
#include <string.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>


static Display *display;
static Window window;
static Window root_window;

static Colormap colormap;
static XEvent report;

static XVisualInfo *x_visual_info;
static XSetWindowAttributes x_set_wnd_attrs;
static XWindowAttributes x_wnd_attrs;

static GLXContext glx_context;
static GLint gl_attrs[] = {
	GLX_RGBA,
	GLX_DEPTH_SIZE,
	24,
	GLX_DOUBLEBUFFER,
	None
};

/*!
 * \brief DrawAQuad
 * \param x
 * \param y
 */
void DrawAQuad(int x, int y) {

	static double xx = 0.0;
	char str[80], font_string[128];
	XFontStruct *font_struct;

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for(int font_size = 14; font_size < 32; font_size += 2) {
		sprintf(font_string, "*mono*--%i-*", font_size);
		font_struct = XLoadQueryFont(display, font_string);

		if(font_struct != NULL) {
				glXUseXFont(font_struct->fid, 32, 192, 32);
				break;
		}
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, 1.0, 20.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		xx, xx, 10.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);

	glBegin(GL_QUADS);
	glColor3f(1.0, 0.0, 0.0); glVertex3f(-0.5, -0.5, 0.0);
	glColor3f(1.0, 1.0, 0.0); glVertex3f( 0.5, -0.5, 0.0);
	glColor3f(1.0, 0.5, 1.0); glVertex3f( 0.5,  0.5, 0.0);
	glColor3f(1.0, 0.1, 0.5); glVertex3f(-0.5,  0.5, 0.0);

	glColor3f(0.0, 1.0, 0.0); glVertex3f(0.0, -0.5, -0.5);
	glColor3f(0.0, 1.0, 0.0); glVertex3f(0.0,  0.5, -0.5);
	glColor3f(0.0, 1.0, 0.0); glVertex3f(0.0,  0.5,  0.5);
	glColor3f(0.0, 1.0, 0.0); glVertex3f(0.0, -0.5,  0.5);

	glEnd();

	xx = xx + 0.1;
	if (xx > 5.0) {
		xx = 0.0;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, (float)x_wnd_attrs.width, 0, (float)x_wnd_attrs.height, -1., 1.);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(1.0, 1.0, 1.0);

	sprintf(str, "x = %d; y = %d;", x, y);
	glRasterPos2i(x, y);
	glCallLists(strlen(str), GL_UNSIGNED_BYTE, str);

}

/*!
 * \brief start_glwnd
 * \param wparam
 * \return
 */
int start_glwnd(wndparam * wparam)
{
	if (wparam == NULL) {
		return -1;
	}

	if ((display = XOpenDisplay(NULL)) == NULL) {
		printf("[%s:%d] Can\'t connect to the X server!\n"
			   , __func__, __LINE__);
		return -1;
	}

	root_window = DefaultRootWindow(display);

	if ((x_visual_info = glXChooseVisual(display, 0, gl_attrs)) == NULL) {
		printf("[%s:%d] no appropriate visual found\n"
			   , __func__, __LINE__);
		return -1;
	}

	printf("[%s:%d] visual %p selected\n", __func__, __LINE__,
		(void *)x_visual_info->visualid);


	if ((colormap = XCreateColormap(display, root_window,
		x_visual_info->visual, AllocNone)) == 0) {
		printf("[%s:%d] cannot create colormap\n"
			   , __func__, __LINE__);
		return -1;
	}

	x_set_wnd_attrs.colormap = colormap;
	x_set_wnd_attrs.event_mask =
		ExposureMask |
		KeyPressMask |
		Button1MotionMask |
		PointerMotionMask;

	window = XCreateWindow(display, root_window,
		wparam->x, wparam->y, wparam->width, wparam->height,
		wparam->border_width, x_visual_info->depth,
		InputOutput, x_visual_info->visual,
		CWColormap | CWEventMask, &x_set_wnd_attrs
	);

	XMapWindow(display, window);
	XStoreName(display, window, "OpenGL test application");

	glx_context = glXCreateContext(display, x_visual_info, NULL, GL_TRUE);
	glXMakeCurrent(display, window, glx_context);
	glEnable(GL_DEPTH_TEST);

	return 0;
}

/*!
 * \brief glwnd
 * \param wparam
 * \return
 */
int glwnd(wndparam *wparam)
{
	if (start_glwnd(wparam) < 0) {
		return -1;
	}

	while (1) {
		XNextEvent(display, &report);

		switch (report.type) {
			case Expose:
				if (report.xexpose.count != 0) {
					break;
				}

				XGetWindowAttributes(display, window, &x_wnd_attrs);
				glViewport(0, 0, x_wnd_attrs.width, x_wnd_attrs.height);
				DrawAQuad(0,0);
				glXSwapBuffers(display, window);

				break;

			case KeyPress:
				glXMakeCurrent(display, None, NULL);
				glXDestroyContext(display, glx_context);
				XDestroyWindow(display, window);
				XCloseDisplay(display);
				return 0;

			case MotionNotify:
				XGetWindowAttributes(display, window, &x_wnd_attrs);
				glViewport(0, 0, x_wnd_attrs.width, x_wnd_attrs.height);
				DrawAQuad(report.xmotion.x, report.xmotion.y);
				glXSwapBuffers(display, window);
				printf("[%s:%d] MotionNotify: x = %d; y = %d\n"
					, __FUNCTION__, __LINE__,
					report.xmotion.x, report.xmotion.y);
				break;

			default:
				break;
		}
	}

	return 0;
}

/*
 --------------------------------------------------------------------
 GC gc;
				gc = XCreateGC(display, window, 0, NULL);

				XSetForeground(display, gc, BlackPixel(display, 0));
				XDrawString(display, window, gc, 20, 50, str, strlen(str));
				XFreeGC(display, gc);
				XFlush(display);
	if ((display = XOpenDisplay(NULL)) == NULL) {
		printf("[%s:%d] Can\'t connect to the X server!\n"
			   , __func__, __LINE__);
		return -1;
	}

	scr_num = DefaultScreen(display);

	window = XCreateSimpleWindow(
		display, RootWindow(display, scr_num),
		100, 100, 640, 480, 0,
		BlackPixel(display, scr_num),
		WhitePixel(display, scr_num));

	XSelectInput(display, window, ExposureMask | KeyPressMask);

	XMapWindow(display, window);
*/
