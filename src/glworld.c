#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <X11/Xlib.h>

#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/time.h>
#include <time.h>


#include "glworld.h"


void initTimer(void);
void updateTimer (void);
void getFPS (void);


GLfloat rot_matrix[16];
float TimeCounter, LastFrameTimeCounter, DT, prevTime = 0.0, FPS;
struct timeval tv, tv0;
int Frame = 1, FramesPerFPS, wnd_width, wnd_height;
float rot_z_vel = 50.0, rot_y_vel = 30.0;
float aspect_ratio;


/*!
 * \brief drawCube
 * \param size
 */
void drawCube (float size) {

	glBegin(GL_QUADS);

	glColor3f(0.7, 0.0, 0.0);
	glVertex3f(-size, -size, -size);
	glVertex3f( size, -size, -size);
	glVertex3f( size,  size, -size);
	glVertex3f(-size,  size, -size);

	glVertex3f(-size, -size,  size);
	glVertex3f( size, -size,  size);
	glVertex3f( size,  size,  size);
	glVertex3f(-size,  size,  size);

	glColor3f(0.0, 0.0, 0.7);

	glVertex3f(-size, -size, -size);
	glVertex3f(-size, -size,  size);
	glVertex3f(-size,  size,  size);
	glVertex3f(-size,  size, -size);

	glVertex3f( size, -size, -size);
	glVertex3f( size, -size,  size);
	glVertex3f( size,  size,  size);
	glVertex3f( size,  size, -size);

	glColor3f(0.0, 0.7, 0.0);

	glVertex3f(-size, -size, -size);
	glVertex3f(-size, -size,  size);
	glVertex3f( size, -size,  size);
	glVertex3f( size, -size, -size);

	glVertex3f(-size, size, -size);
	glVertex3f(-size, size,  size);
	glVertex3f( size, size,  size);
	glVertex3f( size, size, -size);

	glEnd();
}

/*!
 * \brief rotateCube
 */
void rotateCube (void) {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(rot_y_vel * DT, 0.0, 1.0, 0.0);
	glRotatef(rot_z_vel * DT, 0.0, 0.0, 1.0);
	glMultMatrixf(rot_matrix);
	glGetFloatv(GL_MODELVIEW_MATRIX, rot_matrix);
}

static void update(void) {
	char str[256];

	glViewport(0, 0, wnd_width, wnd_height);

	/* SETUP PROJECTION & MODELVIEW */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2.50 * aspect_ratio, 2.50 * aspect_ratio,
		-2.50, 2.50, 1.0, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glMultMatrixf(rot_matrix);

	/* DRAW CUBE */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawCube(1.0);

	/* DISPLAY TIME, FPS etc. */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, (float)wnd_width, 0.0, (float)wnd_height, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(1.0, 1.0, 1.0);

	sprintf(str, "%4.1f seconds * %4.1f fps at %i x %i"
		, TimeCounter
		, FPS
		, wnd_width
		, wnd_height);

	glRasterPos2i(10, 10);
	glCallLists(strlen(str), GL_UNSIGNED_BYTE, str);

	sprintf(str, "<up,down,left,right> rotate cube * <F1> stop rotation ");
	glRasterPos2i(10, wnd_height - 32);
	glCallLists(strlen(str), GL_UNSIGNED_BYTE, str);
}

/*!
 * \brief initializeGL
 */
void initializeGL (void) {
	glEnable(GL_DEPTH_TEST);

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glGetFloatv(GL_MODELVIEW_MATRIX, rot_matrix);

	initTimer();
}

/*!
 * \brief resizeGL
 * \param width
 * \param height
 */
void resizeGL (int width, int height) {
	wnd_width = width;
	wnd_height = height;
	aspect_ratio = (height > 0) ? (float)width / (float)height : 0.0f;
	update();

}

/*!
 * \brief paintGL
 */
void paintGL (void) {
	updateTimer();
	getFPS();
	rotateCube();
	update();
}

/*!
 * \brief keyPressEvent
 * \param keycode
 */
void keyPressEvent (int keycode) {

	switch (keycode) {
	case XK_Up:
		rot_y_vel -= 200.0 * DT;
		break;

	case XK_Left:
		rot_z_vel -= 200.0 * DT;
		break;

	case XK_Right:
		rot_z_vel += 200.0 * DT;
		break;

	case XK_Down:
		rot_y_vel += 200.0 * DT;
		break;

	case XK_F1:
		rot_y_vel = 0.0;
		rot_z_vel = 0.0;
		break;

	default:
		break;
	}
}

void kyeReleaseEvent (void) {

}

void mouseMoveEvent (void) {

}

void mousePressEvent (void) {

}

void mouseReleaseEvent (void) {

}

void mouseWheelEvent (void) {

}

/*!
 * \brief initTimer
 */
void initTimer(void) {
	gettimeofday(&tv0, NULL);
	FramesPerFPS = 30;
}

/*!
 * \brief updateTimer
 */
void updateTimer (void) {
	LastFrameTimeCounter = TimeCounter;
	gettimeofday(&tv, NULL);
	TimeCounter = (float)(tv.tv_sec - tv0.tv_sec) +
		0.000001 * ((float)(tv.tv_usec - tv0.tv_usec));
	DT = TimeCounter - LastFrameTimeCounter;
}

/*!
 * \brief getFPS
 */
void getFPS (void) {
	Frame++;
	if((Frame % FramesPerFPS) == 0) {
		FPS = ((float)(FramesPerFPS)) / (TimeCounter - prevTime);
		prevTime = TimeCounter;
	}
}
