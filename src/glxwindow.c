#include "gdefs.h"
#include "glxwindow.h"
#include "glworld.h"
#include "xeventhandler.h"

#include <stdio.h>
#include <unistd.h>

#include <GL/glx.h>
#include <X11/Xatom.h>


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
static void setupFont (void) {
	char           font_string[128];
	XFontStruct    *font_struct;
	unsigned long ret;


	for(int font_size = 14; font_size < 32; font_size += 2) {
		sprintf(font_string, "-misc-fixed-medium-r-normal--%i-*"
			, font_size);
		font_struct = XLoadQueryFont(display, font_string);

		if(font_struct != NULL) {
			glXUseXFont(font_struct->fid, 32, 192, 32);
			if (XGetFontProperty(font_struct, XA_FONT, &ret)) {
				printf("[%s:%d] Font: %s\n"
					, __func__, __LINE__, XGetAtomName(display,
				(Atom)ret));
			}
			break;
		}
	}
}

/*!
 * \brief start_glwnd
 * \param wparam
 * \return
 */
static int glxWindowInit (GLXWindowParams * wparam) {
	if (wparam == NULL) {
		return -1;
	}

#ifdef USE_X_THREAD
	if (XInitThreads() == 0) {
		printf("[%s:%d] XInitThreads() failed\n"
			, __func__, __LINE__);
		return -1;
	}
#endif

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
		ButtonPressMask |
		ButtonReleaseMask |
		PointerMotionMask;

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
static EventHandlerStatus glxExposeEvent (XEvent *event) {
	if (event->xexpose.count != 0) {
		return EHS_OK;
	}
	XLockDisplay(display);
	XGetWindowAttributes(display, window, &x_wnd_attrs);
	resizeGL(x_wnd_attrs.width, x_wnd_attrs.height);
	glXSwapBuffers(display, window);
	XUnlockDisplay(display);

	return EHS_OK;
}

/*!
 * \brief key_press_event
 * \param event
 * \return
 */
static EventHandlerStatus glxKeyPressEvent (XEvent *event) {
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
 * \brief glxMousePressEvent
 * \param event
 * \return
 */
static EventHandlerStatus glxMousePressEvent (XEvent *event) {
	if (event == NULL) {
		return EHS_OK;
	}

	if (event->type != ButtonPress) {
		return EHS_OK;
	}

	static mouse_event_t mouse_event;
	mouse_event.button = event->xbutton.button;
	mouse_event.x = event->xbutton.x;
	mouse_event.y = event->xbutton.y;
	mousePressEvent(&mouse_event);

	return EHS_OK;
}

static EventHandlerStatus glxMouseReleaseEvent (XEvent *event) {

	if (event == NULL) {
		return EHS_OK;
	}

	if (event->type != ButtonRelease) {
		return EHS_OK;
	}
	static mouse_event_t mouse_event;
	mouse_event.button = event->xbutton.button;
	mouse_event.x = event->xbutton.x;
	mouse_event.y = event->xbutton.y;
	mouseReleaseEvent(&mouse_event);

	return EHS_OK;
}

static EventHandlerStatus glxMouseMoveEvent (XEvent *event) {
	if (event == NULL) {
		return EHS_OK;
	}

	if (event->type != MotionNotify) {
		return EHS_OK;
	}

	mouseMoveEvent(event->xmotion.x, event->xmotion.y);

	return EHS_OK;
}

/*!
 * \brief updateWindow
 * \param event
 * \return
 */
static EventHandlerStatus updateWindow (XEvent *event) {
	UNUSED(event);

	XLockDisplay(display);
	paintGL();
	glXSwapBuffers(display, window);
//	usleep(1000);
	XUnlockDisplay(display);
	return EHS_OK;
}

/*!
 * \brief glwnd
 * \param wparam
 * \return
 */
int startGLXWindow (GLXWindowParams *wparam) {
	int ret = 0;
	if ((ret = glxWindowInit(wparam)) < 0) {
		return ret;
	}

	registerEventCallback(glxExposeEvent, Expose);
	registerEventCallback(glxKeyPressEvent, KeyPress);
	registerEventCallback(glxMousePressEvent, ButtonPress);
	registerEventCallback(glxMouseReleaseEvent, ButtonRelease);
	registerEventCallback(glxMouseMoveEvent, MotionNotify);

	registerLoopCallback(updateWindow);

#ifdef USE_X_THREAD
	EventHandlerStatus status = eventHandlerThreadLoop(display, window);
#else
	EventHandlerStatus status = eventHandlerLoop(display, window);
#endif

	if (status == EHS_FAILED) {
		return -1;
	}

	return 0;
}

/*!
 * \brief stopGLWindow
 */
void stopGLXWindow (void) {
	glXMakeCurrent(display, None, NULL);
	glXDestroyContext(display, glx_context);
	XDestroyWindow(display, window);
	XCloseDisplay(display);
}
