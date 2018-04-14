#ifndef EH_H
#define EH_H


#include <X11/Xlib.h>


typedef enum {
	EHS_OK = 1,
	EHS_EXIT = 0,
	EHS_FAILED = -1
} EventHandlerStatus;

typedef EventHandlerStatus (*EventCallback)(XEvent *event);


int RegisterEventCallback(EventCallback callback, int type);
EventHandlerStatus EventHandlerLoop(Display *disp);

#endif // EH_H
