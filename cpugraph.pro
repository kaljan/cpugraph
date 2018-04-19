TEMPLATE = app
CONFIG -= qt
CONFIG += x11
CONFIG += opengl
CONFIG += pthread

LIBS += -lGLU
LIBS += -L/usr/lib/xorg/modules/extensions/
LIBS += -lglx
LIBS += -lpthread

QMAKE_CFLAGS += -pthread

INCLUDEPATH += ./src/

SOURCES += \
	src/main.c \
	src/xeventhandler.c \
	src/glxwindow.c \
	src/glworld.c

HEADERS += \
	src/gdefs.h \
	src/xeventhandler.h \
	src/glxwindow.h \
	src/glworld.h

DISTFILES += \
	src/temp.txt

