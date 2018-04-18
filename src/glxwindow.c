#include "gdefs.h"
#include "glxwindow.h"
#include "glworld.h"
#include "xeventhandler.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>


static Display *display;
static Window window;
static Window root_window;

static Colormap colormap;

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
 * \brief setupFont
 */
static void setupFont(void) {
	char           font_string[128];
	XFontStruct    *font_struct;

	int fnts = 0;

	char **fontlist = XListFonts(display, "-*-*-*-*-*-*-*-*-*-*-*-10", 100, &fnts);

	printf("[%s:%d] Fonts %d found\n"
		, __func__, __LINE__, fnts);

	if (fontlist != NULL) {
		while (fnts--) {
			printf("[%s:%d] Font: %s\n"
				, __func__, __LINE__, *fontlist);
			fontlist++;
		}
	}

	for(int font_size = 14; font_size < 32; font_size += 2) {
		sprintf(font_string, "-*-*-*-r-*-*-%i-*", font_size);
		font_struct = XLoadQueryFont(display, font_string);

		printf("[%s:%d] Search font: %s\n"
			, __func__, __LINE__, font_string);

		if(font_struct != NULL) {
			printf("[%s:%d] Font \'%s\' found\n"
				, __func__, __LINE__, font_string);

			glXUseXFont(font_struct->fid, 32, 192, 32);
			break;
		}
		printf("[%s:%d] Font \'%s\' not found\n"
			, __func__, __LINE__, font_string);
	}
}

/*!
 * \brief start_glwnd
 * \param wparam
 * \return
 */
static int xGLWindowInit(GLXWindowParams * wparam) {
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
		KeyPressMask;

	window = XCreateWindow(display, root_window,
		wparam->x, wparam->y, wparam->width, wparam->height,
		wparam->border_width, x_visual_info->depth,
		InputOutput, x_visual_info->visual,
		CWColormap | CWEventMask, &x_set_wnd_attrs
	);

	XMapWindow(display, window);
	XStoreName(display, window, "OpenGL test application");

	if ((glx_context = glXCreateContext(display,
		x_visual_info, NULL, GL_TRUE)) == NULL) {
		printf("[%s:%d] cannot create gl context\n", __func__, __LINE__);
		return -1;
	}
	glXMakeCurrent(display, window, glx_context);

	setupFont();
	initializeGL();

	return 0;
}

/*!
 * \brief expose_event
 * \param event
 * \return
 */
EventHandlerStatus expose_event(XEvent *event) {
	if (event->xexpose.count != 0) {
		return EHS_OK;
	}

	XGetWindowAttributes(display, window, &x_wnd_attrs);
	resizeGL(x_wnd_attrs.width, x_wnd_attrs.height);
	glXSwapBuffers(display, window);

	return EHS_OK;
}

/*!
 * \brief key_press_event
 * \param event
 * \return
 */
EventHandlerStatus key_press_event(XEvent *event) {
//	UNUSED(event);

	if (event == NULL) {
		return EHS_OK;
	}

	if (event->type != KeyPress) {
		return EHS_OK;
	}

	int kcode = XLookupKeysym(&event->xkey, 0);

	if (kcode == XK_Escape) {
		stopGLXWindow();
		return EHS_EXIT;
	}

	keyPressEvent(kcode);
	return EHS_OK;
}

/*!
 * \brief updateWindow
 * \param event
 * \return
 */
EventHandlerStatus updateWindow(XEvent *event) {
	UNUSED(event);

	paintGL();
	glXSwapBuffers(display, window);
	usleep(1000);
	return EHS_OK;
}

/*!
 * \brief glwnd
 * \param wparam
 * \return
 */
int startGLXWindow(GLXWindowParams *wparam) {
	int ret = 0;
	if ((ret = xGLWindowInit(wparam)) < 0) {
		return ret;
	}

	RegisterEventCallback(expose_event, Expose);
	RegisterEventCallback(key_press_event, KeyPress);

	registerLoopCallback(updateWindow);

	EventHandlerStatus status = EventHandlerLoop(display, window);
	if (status == EHS_EXIT) {
		return 0;
	}

	if (status == EHS_FAILED) {
		return -1;
	}

	return ret;
}

/*!
 * \brief stopGLWindow
 */
void stopGLXWindow(void) {
	glXMakeCurrent(display, None, NULL);
	glXDestroyContext(display, glx_context);
	XDestroyWindow(display, window);
	XCloseDisplay(display);
}

