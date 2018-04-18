#include "xeventhandler.h"
#include "gdefs.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


EventCallback e_callback[38] = {NULL};
XEvent event;
EventCallback loop_callback = NULL;


/*!
 * \brief registerLoopCallback
 * \param callback
 */
void registerLoopCallback(EventCallback callback) {
	loop_callback = callback;
}

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
 * \brief event_check_handler
 * \param event
 * \param disp
 * \param wnd
 * \param type
 * \param clbk
 * \return
 */
static EventHandlerStatus event_check_handler(
	XEvent *event, Display *disp, Window wnd, int type, EventCallback clbk) {
	if (type < 2 || type > 36) {
		return EHS_OK;
	}
	if (XCheckTypedWindowEvent(disp, wnd, type, event)) {
		if (clbk != NULL) {
			return (*clbk)(event);
		}
	}
	return EHS_OK;

}

/*!
 * \brief event_loop
 * \param event
 * \param disp
 * \param wnd
 * \return
 */
static EventHandlerStatus event_loop(XEvent *event, Display *disp, Window wnd) {
	EventHandlerStatus ret = EHS_OK;
	for (int i = 2; i < 37; i++) {
		if ((ret = event_check_handler(event, disp, wnd,
			i, e_callback[i])) != EHS_OK) {
			break;
		}
	}
	return ret;
}

/*!
 * \brief EventLoop
 * \param disp
 * \return
 */
EventHandlerStatus EventHandlerLoop(Display *disp, Window wnd) {
	EventHandlerStatus ret = EHS_OK;
	if (disp == NULL) {
		return EHS_FAILED;
	}

	while (1) {
		if (loop_callback != NULL) {
			(*loop_callback)(&event);
		}

		if ((ret = event_loop(&event, disp, wnd)) != EHS_OK) {
			break;
		}
	}
	return ret;
}
