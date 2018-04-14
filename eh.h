#ifndef EH_H
#define EH_H

#include <X11/Xlib.h>

typedef enum {
	HE_DRAW=2,
	HE_EXIT=1,
	HE_NOP=0,
	HE_FAILED=-1
} handler_event;

typedef void *(*event_callback)(void *);

#endif // EH_H
