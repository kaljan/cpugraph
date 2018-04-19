#include "gdefs.h"
#include "glxwindow.h"

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int thr_state = 0;
pthread_t thread;

void *my_thread(void *arg) {
	int i = 0;
	printf("thread\n");
	while (1) {
		printf("i = %d;\n", i++);
		usleep(10000);
		if (i > 10000) {
			thr_state = 1;
			break;
		}
	}
	return arg;
}

int main(int argc, char **argv)
{
	UNUSED(argc);
	UNUSED(argv);
#if 0
	int ret;
	ret = pthread_create(&thread, NULL, my_thread, NULL);
	if (ret < 0) {
		printf("create thread failed; %d\n", ret);
		return -1;
	}

	while (1) {
		if (thr_state != 0) {
			printf("exit\n");
			break;
		}
	}
#endif

	GLXWindowParams wp;
	wp.x = 100;
	wp.y = 100;
	wp.width = 640;
	wp.height = 480;
	wp.border_width = 0;

	return startGLXWindow(&wp);
}
