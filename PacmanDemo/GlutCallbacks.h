#ifndef _GLUT_CALLBACKS_H
#define _GLUT_CALLBACKS_H

#include "GameWorld.h"

static GameWorld world;

// GLUT callback funkcije

static void displayCallback(void) {
	world.display();
}

static void reshapeCallback(int p_w, int p_h) {
	world.reshape(p_w, p_h);
}

static void idleCallback(void) {
	world.idle();
}

static void keyboardSpecialCallback(int p_key, int p_x, int p_y) {
	world.keyboardSpec(p_key, p_x, p_y);
}

static void keyboardSpecialUpCallback(int p_key, int p_x, int p_y) {
	world.keyboardSpecUp(p_key, p_x, p_y);
}

static void keyboardCallback(unsigned char p_key, int p_x, int p_y) {
	world.keyboard(p_key, p_x, p_y);
}

static void keyboardUpCallback(unsigned char p_key, int p_x, int p_y) {
	world.keyboardUp(p_key, p_x, p_y);
}

static void joystickCallback(unsigned int p_buttons, int p_x, int p_y, int p_z) {
	world.joystick(p_buttons, p_x, p_y, p_z);
}

static void mouseCallback(int p_button, int p_state, int p_x, int p_y) {
	world.mouse(p_button, p_state, p_x, p_y);
}

// ------------------------------------------------------------------------------

int glutMain(int p_argc, char** p_argv, int p_width, int p_height, const char* p_title) {

	world = GameWorld(); // alociranje memorije za objekat klase GameWorld

	glutInit(&p_argc, p_argv);	// inicijalizacija GLUT biblioteke
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // podešavanje prozora prema zadatim parametrima
	glutInitWindowPosition(windowPositionX, windowPositionY); // postavljanje veličine prozora
	glutInitWindowSize(p_width, p_height); // podešavanje udaljenosti prozora od gornjeg levog ugla
	glutCreateWindow(p_title); // kreiranje prozora sa datim nazivom

	world.init(); // pozivanje funkcije za inicijalizaciju sveta igre

	glutDisplayFunc(displayCallback); // postavljanje display callback-a
	glutReshapeFunc(reshapeCallback); // postavljanje reshape callback-a
	glutIdleFunc(idleCallback);		  // postavljanje idle callback-a

	// postavljanje callback funkcija za događaje miša, tastature i dzojstika
	glutSpecialUpFunc(keyboardSpecialUpCallback); 
	glutSpecialFunc(keyboardSpecialCallback);
	glutKeyboardFunc(keyboardCallback);
	glutKeyboardUpFunc(keyboardUpCallback);
	glutJoystickFunc(joystickCallback, joystickPollInterval);
	glutMouseFunc(mouseCallback);
	// ------------------------------------------------------

	glutMainLoop(); // glavna petlja igre

	return 0;
}
#endif