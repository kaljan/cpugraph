#include "xeventhandler.h"
#include "gdefs.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


EventCallback e_callback[38] = {NULL};
XEvent event;


/*!
 * \brief RegisterEventCallback
 * \param callback
 * \param type
 * \return
 */
int RegisterEventCallback(EventCallback callback, int type) {
	if (callback == NULL) {
		return -1;
	}

	if (type < 2 || type > 36) {
		return -1;
	}
	e_callback[type] = callback;

	return 0;
}

/*!
 * \brief event_handler
 * \param event
 * \param disp
 * \return
 */
static EventHandlerStatus event_handler(XEvent *event, Display *disp) {
	if (event == NULL) {
		return EHS_FAILED;
	}

	XNextEvent(disp, event);

	int type = event->type;
	if (type < 2 || type > 36) {
		return EHS_OK;
	}

	EventCallback clbk = e_callback[type];
	if (clbk != NULL) {
		return (*clbk)(event);
	}

	return EHS_OK;
}

/*!
 * \brief EventLoop
 * \param disp
 * \return
 */
EventHandlerStatus EventHandlerLoop(Display *disp) {
	EventHandlerStatus ret = EHS_OK;
	if (disp == NULL) {
		return EHS_FAILED;
	}

	while (1) {
		ret = event_handler(&event, disp);
		if (ret != EHS_OK) {
			break;
		}
	}
	return ret;
}


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


event_callback key_press_callback;
event_callback key_release_callback;
event_callback button_press_callback;
event_callback button_release_callback;
event_callback motion_notify_callback;
event_callback enter_notify_callback;
event_callback leave_notify_callback;
event_callback focus_in_callback;
event_callback focus_out_callback;
event_callback key_map_notify_callback;
event_callback expose_callback;
event_callback graphic_expose_callback;
event_callback no_expose_callback;
event_callback visibility_notify_callback;
event_callback create_notify_callback;
event_callback destroy_notify_callback;
event_callback unmap_notify_callback;
event_callback map_notify_callback;
event_callback map_request_callback;
event_callback reparent_notyfy_callback;
event_callback configure_notify_callback;
event_callback configure_request_callback;
event_callback gravity_notify_callback;
event_callback resize_request_callback;
event_callback circulate_notify_callback;
event_callback circulate_request_callback;
event_callback property_notify_callback;
event_callback selection_clear_callback;
event_callback selection_request_callback;
event_callback selection_notify_callback;
event_callback colormap_notify_callback;
event_callback client_message_callback;
event_callback mapping_notify_callback;
event_callback generic_event_callback;
event_callback last_event_callback;
	switch (event) {

	case KeyPress:
		key_press_callback = callback;
		break;

	case KeyRelease:
		key_release_callback = callback;
		break;

	case ButtonPress:
		button_press_callback = callback;
		break;

	case ButtonRelease:
		button_release_callback = callback;
		break;

	case MotionNotify:
		motion_notify_callback = callback;
		break;

	case EnterNotify:
		enter_notify_callback = callback;
		break;

	case LeaveNotify:
		leave_notify_callback = callback;
		break;

	case FocusIn:
		focus_in_callback = callback;
		break;

	case FocusOut:
		focus_out_callback = callback;
		break;

	case KeymapNotify:
		key_map_notify_callback = callback;
		break;

	case Expose:
		expose_callback = callback;
		break;

	case GraphicsExpose:

		break;

	case NoExpose:
		break;

	case VisibilityNotify:
		break;

	case CreateNotify:
		break;

	case DestroyNotify:
		break;

	case UnmapNotify:
		break;

	case MapNotify:
		break;

	case MapRequest:
		break;

	case ReparentNotify:
		break;

	case ConfigureNotify:
		break;

	case ConfigureRequest:
		break;

	case GravityNotify:
		break;

	case ResizeRequest:
		break;

	case CirculateNotify:
		break;

	case CirculateRequest:
		break;

	case PropertyNotify:
		break;

	case SelectionClear:
		break;

	case SelectionRequest:
		break;

	case SelectionNotify:
		break;

	case ColormapNotify:
		break;

	case ClientMessage:
		break;

	case MappingNotify:
		break;

	case GenericEvent:
		break;

	case LASTEvent:
		break;

	default:
		return -1;
	}

	switch (event->type) {
	case KeyPress: break;
	case KeyRelease: break;
	case ButtonPress: break;
	case ButtonRelease: break;
	case MotionNotify: break;
	case EnterNotify: break;
	case LeaveNotify: break;
	case FocusIn: break;
	case FocusOut: break;
	case KeymapNotify: break;
	case Expose: break;
	case GraphicsExpose: break;
	case NoExpose: break;
	case VisibilityNotify: break;
	case CreateNotify: break;
	case DestroyNotify: break;
	case UnmapNotify: break;
	case MapNotify: break;
	case MapRequest: break;
	case ReparentNotify: break;
	case ConfigureNotify: break;
	case ConfigureRequest: break;
	case GravityNotify: break;
	case ResizeRequest: break;
	case CirculateNotify: break;
	case CirculateRequest: break;
	case PropertyNotify: break;
	case SelectionClear: break;
	case SelectionRequest: break;
	case SelectionNotify: break;
	case ColormapNotify: break;
	case ClientMessage: break;
	case MappingNotify: break;
	case GenericEvent: break;
	case LASTEvent: break;

	default:
		break;
	}
*/
