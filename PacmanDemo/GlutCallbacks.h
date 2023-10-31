#ifndef _GLUT_CALLBACKS_H
#define _GLUT_CALLBACKS_H

#include "GameWorld.h"

static GameWorld world;

static void displayCallback(void) {

}

static void reshapeCallback(int p_w, int p_h) {
	//world->reshape(p_w, p_h);
}

static void idleCallback(void) {
	//world->idle();
}

static void keyboardCallback(unsigned char p_key, int p_x, int p_y) {
	//world->keyboard(p_key, p_x, p_y);
}

static void keyboardUpCallback(unsigned char p_key, int p_x, int p_y) {
	//world->keyboardUp(p_key, p_x, p_y);
}

static void mouseCallback(int p_button, int p_state, int p_x, int p_y) {
	//world->mouse(p_button, p_state, p_x, p_y);
}

int glutMain(int p_argc, char** p_argv, int p_width, int p_height, const char* p_title, GameWorld p_world) {

	world = p_world;

	glutInit(&p_argc, p_argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(p_width, p_height);
	glutCreateWindow(p_title);


	glutDisplayFunc(displayCallback);
	glutReshapeFunc(reshapeCallback);
	glutIdleFunc(idleCallback);

	glutKeyboardFunc(keyboardCallback);
	glutKeyboardUpFunc(keyboardUpCallback);
	glutMouseFunc(mouseCallback);

	glutMainLoop();

	return 0;
}

#endif