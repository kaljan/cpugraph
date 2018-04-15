TEMPLATE = app
CONFIG -= qt
CONFIG += x11
CONFIG += opengl

LIBS += -lGLU
LIBS += -L/usr/lib/xorg/modules/extensions/
LIBS += -lglx

<<<<<<< HEAD
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
=======
SOURCES += main.c \
    xeventhandler.c \
    glxwindow.c \
    glworld.c

HEADERS += \
	gdefs.h \
    xeventhandler.h \
    glxwindow.h \
    glworld.h
>>>>>>> 3442b93266bd11ba9c34b6e1de980e0cf6ef65b7
