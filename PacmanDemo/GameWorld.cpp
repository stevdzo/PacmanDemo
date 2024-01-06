#include "GameWorld.h"
#include "Graph.h"
#include "InputManager.h"
#include "AudioManager.h"
#include "TextRenderer.h"

void GameWorld::init() {
	std::cout << "***********************************" << std::endl;
	std::cout << "*** OpenGL Pacman demo ~ Stevan ***" << std::endl;
	std::cout << "***********************************" << std::endl;

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glDisable(GL_DEPTH_TEST);	

	srand(time(0));

	m_graph = Graph::getInstance();
	
	initDots();

	m_background = new GameObject(Sprite(mazeFilePath, 1, 1, false));
	m_background->setPosition(Vector2D((screenWidth-400) / 2, screenHeight / 2));
	m_background->setSize(Vector2D(screenWidth-400, screenHeight));

	m_player = new Player(Sprite(pacFilePath, 3, 4));

	m_blinky = new Enemy(GhostType::blinky, Sprite(blinkyFilePath, 2, 8), blinkyScatterNodeIndices, blinkyBaseNodeIndices, m_player);
	m_pinky  = new Enemy(GhostType::pinky , Sprite(pinkyFilePath , 2, 8), pinkyScatterNodeIndices , pinkyBaseNodeIndices , m_player);
	m_inky   = new Enemy(GhostType::inky  , Sprite(inkyFilePath  , 2, 8), inkyScatterNodeIndices  , inkyBaseNodeIndices  , m_player);
	m_clyde  = new Enemy(GhostType::clyde , Sprite(clydeFilePath , 2, 8), clydeScatterNodeIndices , clydeBaseNodeIndices , m_player); 

	m_cherry = new Drop(Sprite(cherryFilePath, 1, 1));

	m_blinky->setPositionByNode(blinkyStartNodeIndex);
	m_pinky->setPositionByNode(pinkyStartNodeIndex);
	m_inky->setPositionByNode(inkyStartNodeIndex);
	m_clyde->setPositionByNode(clydeStartNodeIndex);

	m_blinky->setCurrentDirection(Direction::left);
	m_pinky->setCurrentDirection(Direction::down);
	m_inky->setCurrentDirection(Direction::up);
	m_clyde->setCurrentDirection(Direction::down);

	m_blinky->changeEnemyState(EnemyState::scatter);
	m_pinky->changeEnemyState(EnemyState::base);
	m_inky->changeEnemyState(EnemyState::base);
	m_clyde->changeEnemyState(EnemyState::base);

	m_inky->assignBlinkyToInky(m_blinky);

	m_ghosts.push_back(m_blinky);
	m_ghosts.push_back(m_pinky);
	m_ghosts.push_back(m_inky);
	m_ghosts.push_back(m_clyde);

	m_inputManager = InputManager::getInstance(m_player);
	m_audioManager = AudioManager::getInstance();

	m_audioManager->playIntroSound();

	astar = AStar();
}

void GameWorld::initDots() {
	for (size_t row = 0; row < rows; row++) {
		for (size_t col = 0; col < columns; col++) {
			if (dots[row][col] == 1) {
				Dot* dot = new Dot(Sprite(smallDotFilePath, 1, 1), DotType::small);
				dot->setPosition(Vector2D(row * nodeSize + dot->getSize().x * 2, col * nodeSize + dot->getSize().y * 2));
				m_dots.push_back(dot);
			}
			if (dots[row][col] == 2) {
				Dot* dot = new Dot(Sprite(bigDotFilePath, 1, 1), DotType::big);
				dot->setPosition(Vector2D(row * nodeSize + dot->getSize().x / 2, col * nodeSize + dot->getSize().y / 2));
				m_dots.push_back(dot);
			}
		}
	}
}

void GameWorld::update(float p_deltaTime) {

	if (!gameActive) {
		gameStartTimer += p_deltaTime;

		/*if (!AudioManager::getInstance()->isPlaying(AudioManager::getInstance()->m_chDie)) {
			restart();
		}*/
	}

	if (gameStartTimer > gameStartTimerThreshold) {
		gameActive = true;

		m_pinky->shouldExitBase(true);

		gameStartTimer = 0.0f;
	}

	/*astar.findShortestPath(Graph::getInstance()->getMatrixAsVector(Graph::getInstance()->getNodes())[388], 
						   Graph::getInstance()->getMatrixAsVector(Graph::getInstance()->getNodes())[453]);*/

	if (gameActive) {

		if (!toggleFrightenedMode) {
			globalTimer += p_deltaTime;

			for (auto& ghost : m_ghosts) {
				ghost->manageStateBasedOnTimer();
			}
		}

		m_clyde->update(p_deltaTime);
		//m_inky->update(p_deltaTime);  
		m_pinky->update(p_deltaTime);
		m_blinky->update(p_deltaTime);			

		m_player->update(p_deltaTime);

		m_cherry->update(p_deltaTime);

		m_player->eatDot(m_dots);

		if (globalTimer > 2)
			m_clyde->shouldExitBase(true);

		if (toggleFrightenedMode) {
			for (auto& ghost : m_ghosts)
				if (ghost->getCurrentMode() != EnemyState::eaten) {
					ghost->changeEnemyState(EnemyState::frightened);
				}
			frightenedTimer += p_deltaTime;
		}

		if (frightenedTimer > frightenedTimerThreshold) {

			for (auto& ghost : m_ghosts)
				if (ghost->getCurrentMode() != EnemyState::eaten) {
					ghost->returnPreviousEnemyState();
				}

			if (AudioManager::getInstance()->isPlaying(AudioManager::getInstance()->m_chFrightened)) {
				AudioManager::getInstance()->m_chFrightened->stop();
			}
			frightenedTimer = 0.0f;
			currentBigDotGhostCounter = 1;

			toggleFrightenedMode = false;
		}

		for (auto& ghost : m_ghosts) {
			m_player->onGhostCollision(ghost);
		}
	}
}

void GameWorld::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_background->render();

	for (auto& dot : m_dots) {

		dot->renderWireframe();
		dot->render();
	}	

	m_clyde->render();
	m_inky->render();
	m_pinky->render();
	m_blinky->render();

	m_player->render();

	//m_cherry->render();

	//astar.render();

	renderWireframe();
	renderUi();

	glutSwapBuffers();
}

void GameWorld::renderWireframe() {

	m_graph->renderWireframe();

	m_clyde->renderWireframe();
	m_inky->renderWireframe();
	m_pinky->renderWireframe();
	m_blinky->renderWireframe();

	m_player->renderWireframe();

	//m_cherry->renderWireframe();
}

void GameWorld::gameOver() {
}

void GameWorld::restart() {

	m_blinky->setPositionByNode(blinkyStartNodeIndex);
	m_pinky->setPositionByNode(pinkyStartNodeIndex);
	m_inky->setPositionByNode(inkyStartNodeIndex);
	m_clyde->setPositionByNode(clydeStartNodeIndex);

	m_player->setDefaultPosition();

	if (!AudioManager::getInstance()->isPlaying(AudioManager::getInstance()->m_chIntro)) {
		m_audioManager->playIntroSound();
	}
}

void GameWorld::renderUi() {

	TextRenderer::getInstance()->drawStrokeText((char*) "HIGH SCORE", screenWidth - 200 - 50, screenHeight / 2 + 50, 1, 1, 0);

	char score[50];
	sprintf_s(score, 50, "%i", static_cast<int>(m_player->getScore()));
	TextRenderer::getInstance()->drawStrokeText(score, screenWidth - 200, screenHeight/2, 1, 1, 0);

	char timer[50];
	sprintf_s(timer, 50, "%.2f", globalTimer);
	TextRenderer::getInstance()->drawStrokeText(timer, screenWidth - 200, screenHeight / 2 - 50, 1, 1, 0);
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