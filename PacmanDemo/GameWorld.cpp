#include "GameWorld.h"
#include "Graph.h"
#include "InputManager.h"

void GameWorld::init() {
	std::cout << "***********************************" << std::endl;
	std::cout << "*** OpenGL Pacman demo - Stevan ***" << std::endl;
	std::cout << "***********************************" << std::endl;

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glDisable(GL_DEPTH_TEST);	

	m_graph = Graph::getInstance();
	
	m_player = new Player();

	m_inputManager = InputManager::getInstance(m_player);
}

void GameWorld::update(float p_deltaTime) {
	m_player->update(p_deltaTime);
}

void GameWorld::render() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_graph->renderWireframe();

	m_player->renderWireframe();

	glutSwapBuffers();
}

void GameWorld::keyboard(int p_key, int p_x, int p_y) {
	m_inputManager->keyboard(p_key, p_x, p_y);
}

void GameWorld::keyboardUp(int p_key, int p_x, int p_y) {
	m_inputManager->keyboardUp(p_key, p_x, p_y);
}

void GameWorld::mouse(int p_button, int p_state, int p_x, int p_y) {
	m_inputManager->mouse(p_button, p_state, p_x, p_y);
}

void GameWorld::reshape(int p_w, int p_h) {

	GLsizei screenWidth = p_w;
	GLsizei screenHeight = p_h;

	if (screenHeight == 0) screenHeight = 1;

	glViewport(0, 0, screenWidth, screenHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0f, screenWidth, 0.0f, screenHeight);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GameWorld::display() {
}

void GameWorld::idle() {
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	m_deltaTime = (GLfloat)(currentTime - m_previousTime) / 1000;
	m_previousTime = currentTime;

	update(m_deltaTime);
	render();
}
