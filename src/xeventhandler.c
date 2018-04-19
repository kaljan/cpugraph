#include "xeventhandler.h"
#include "gdefs.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include <pthread.h>


EventCallback e_callback[38] = {NULL};
XEvent x_event;
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
int registerEventCallback(EventCallback callback, int type) {
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
static EventHandlerStatus pollEvent (XEvent *event,
	Display *disp, Window wnd, int type, EventCallback clbk) {
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
static EventHandlerStatus pollEventLoop (XEvent *event,
	Display *disp, Window wnd) {
	EventHandlerStatus ret = EHS_OK;
	for (int i = 2; i < 37; i++) {
		if ((ret = pollEvent(event, disp, wnd,
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
EventHandlerStatus eventHandlerLoop(Display *disp, Window wnd) {
	EventHandlerStatus ret = EHS_OK;
	if (disp == NULL) {
		return EHS_FAILED;
	}

	while (1) {
		if (loop_callback != NULL) {
			(*loop_callback)(&x_event);
		}

		if ((ret = pollEventLoop(&x_event, disp, wnd)) != EHS_OK) {
			break;
		}
	}
	return ret;
}


Display *display = NULL;
Window window = 0;

pthread_t thread;
EventHandlerStatus thread_state = EHS_OK;

/*!
 * \brief event_check_handler
 * \param event
 * \param disp
 * \param wnd
 * \param type
 * \param clbk
 * \return
 */
static EventHandlerStatus threadEvent (XEvent *event,
	Display *disp, Window wnd, int type, EventCallback clbk) {
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

/**
 * @brief treadEventLoop
 * @param event
 * @param disp
 * @param wnd
 * @return
 */
static EventHandlerStatus treadEventLoop (XEvent *event,
	Display *disp, Window wnd) {
//	printf("[%s:%d] --- \n", __func__, __LINE__);
	EventHandlerStatus ret = EHS_OK;
	for (int i = 2; i < 37; i++) {
		if ((ret = threadEvent(event, disp, wnd,
			i, e_callback[i])) != EHS_OK) {
			break;
		}
	}
	return ret;
}

/*!
 * \brief evendHandlerThread
 * \param arg
 * \return
 */
void *evendHandlerThread(void *arg) {
	printf("[%s:%d] --- \n", __func__, __LINE__);
	while (1) {
		printf("[%s:%d] --- \n", __func__, __LINE__);
		if ((thread_state = treadEventLoop(&x_event, display, window)) != EHS_OK) {
			printf("[%s:%d] --- \n", __func__, __LINE__);
			break;
		}
	}
	return arg;
}

/*!
 * \brief eventHandlerThreadLoop
 * \param disp
 * \param wnd
 * \return
 */
EventHandlerStatus eventHandlerThreadLoop(Display *disp, Window wnd) {

	int t_ret;
	if (disp == NULL) {
		return EHS_FAILED;
	}

	printf("[%s:%d] --- \n", __func__, __LINE__);
	display = disp;
	window = wnd;

	printf("[%s:%d] --- \n", __func__, __LINE__);
	if (!(t_ret = pthread_create(&thread, NULL, evendHandlerThread, NULL))) {
		errno = t_ret;
		printf("[%s:%d] Tread create failed! %s\n"
			, __func__, __LINE__, strerror(errno));
		return EHS_FAILED;
	}
	printf("[%s:%d] --- \n", __func__, __LINE__);

	while (1) {
		if (loop_callback != NULL) {
			(*loop_callback)(&x_event);
		}

		if (thread_state != EHS_OK) {
			return thread_state;
		}
	}
	return EHS_OK;
}
