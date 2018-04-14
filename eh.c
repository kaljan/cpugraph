#include "eh.h"
#include "gdefs.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <X11/Xlib.h>

/*
Keyboard events
	KeyPress,
	KeyRelease

Pointer events
	ButtonPress,
	ButtonRelease,
	MotionNotify

Window crossing events
	EnterNotify,
	LeaveNotify

Input focus events
	FocusIn,
	FocusOut

Keymap state notification event
	KeymapNotify

Exposure events
	Expose,
	GraphicsExpose,
	NoExpose

Structure control events
	CirculateRequest,
	ConfigureRequest,
	MapRequest,
	ResizeRequest

Window state notification events
	CirculateNotify,
	ConfigureNotify,
	CreateNotify,
	DestroyNotify,
	GravityNotify,
	MapNotify,
	MappingNotify,
	ReparentNotify,
	UnmapNotify,
	VisibilityNotify

Colormap state notification event
	ColormapNotify

Client communication events
	ClientMessage,
	PropertyNotify,
	SelectionClear,
	SelectionNotify,
	SelectionRequest
*/

event_callback key_event_callback = NULL;
event_callback expose_event_callback = NULL;

handler_event event_handler(XEvent *event) {
	if (event == NULL) {
		return HE_FAILED;
	}

	switch (event->type) {
		case Expose:
			break;

		case ConfigureNotify:
			break;

		case KeyPress:
			if (key_event_callback != NULL) {
				(*key_event_callback)(&(event->xkey));
			}
			break;

		default:
			break;
	}
	return HE_NOP;
}

int reg_xkey_callback(event_callback callback)
{
	if (callback == NULL) {
		return -1;
	}

	key_event_callback = callback;

	return 0;
}

int reg_expose_callback(event_callback callback)
{
	if (callback == NULL) {
		return -1;
	}

	expose_event_callback = callback;

	return 0;
}

int event_loop(void)
{
	return 0;
}
