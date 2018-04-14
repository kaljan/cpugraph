TEMPLATE = app
CONFIG -= qt
CONFIG += x11
CONFIG += opengl

LIBS += -lGLU
LIBS += -L/usr/lib/xorg/modules/extensions/
LIBS += -lglx

SOURCES += main.c \
    xeventhandler.c \
    glxwindow.c

HEADERS += \
	gdefs.h \
    xeventhandler.h \
    glxwindow.h
