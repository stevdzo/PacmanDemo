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

	srand(time(NULL));

	m_graph = Graph::getInstance();
	
	initDots();

	m_player = new Player();

	m_enemy = new Enemy(m_player);	

	m_inputManager = InputManager::getInstance(m_player);
}

void GameWorld::initDots() {
	for (size_t row = 0; row < gv::rows; row++) {
		for (size_t col = 0; col < gv::columns; col++) {
			if (gv::dots[row][col] == 1) {
				Dot* dot = new Dot(DotType::small);
				dot->setPosition(Vector2D(row * gv::nodeSize + dot->getSize().x * 2, col * gv::nodeSize + dot->getSize().y * 2));
				m_dots.push_back(dot);
			}
			if (gv::dots[row][col] == 2) {
				Dot* dot = new Dot(DotType::big);
				dot->setPosition(Vector2D(row * gv::nodeSize + dot->getSize().x / 2, col * gv::nodeSize + dot->getSize().y / 2));
				m_dots.push_back(dot);
			}
		}
	}
}

void GameWorld::update(float p_deltaTime) {
	m_player->update(p_deltaTime);
	m_enemy->update(p_deltaTime);

	m_player->eatDot(m_dots);
}

void GameWorld::render() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_graph->renderWireframe();

	for (auto& dot : m_dots) {

		dot->renderWireframe();
	}

	
	m_enemy->renderWireframe();

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