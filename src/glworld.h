#ifndef GLWORLD_H
#define GLWORLD_H


#include "glxwindow.h"


void initializeGL (void);
void resizeGL (int width, int height);
void paintGL (void);

void keyPressEvent (int keycode);
void mousePressEvent (mouse_event_t *event);
void mouseReleaseEvent (mouse_event_t *event);
void mouseMoveEvent (int x, int y);

#endif // GLWORLD_H
