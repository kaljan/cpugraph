TEMPLATE = app
CONFIG -= qt
CONFIG += x11
CONFIG += opengl

LIBS += -lGLU
LIBS += -L/usr/lib/xorg/modules/extensions/
LIBS += -lglx

SOURCES += main.c \
	eh.c \
	glwnd.c

HEADERS += \
	eh.h \
	glwnd.h \
	gdefs.h
