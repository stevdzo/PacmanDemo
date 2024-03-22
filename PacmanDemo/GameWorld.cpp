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

	m_player = new Player(Sprite(assetsFilePath));

	m_blinky = new Enemy(GhostType::blinky, Sprite(assetsFilePath), blinkyScatterNodeIndices, blinkyBaseNodeIndices, blinkyAnimRange, blinkyInitialState, m_player);
	m_pinky  = new Enemy(GhostType::pinky , Sprite(assetsFilePath), pinkyScatterNodeIndices , pinkyBaseNodeIndices , pinkyAnimRange , pinkyInitialState , m_player);
	m_inky   = new Enemy(GhostType::inky  , Sprite(assetsFilePath), inkyScatterNodeIndices  , inkyBaseNodeIndices  , inkyAnimRange  , inkyInitialState  , m_player);
	m_clyde  = new Enemy(GhostType::clyde , Sprite(assetsFilePath), clydeScatterNodeIndices , clydeBaseNodeIndices , clydeAnimRange , clydeInitialState , m_player);

	//m_cherry = new Drop(Sprite(cherryFilePath));

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

	m_player->setGhosts(m_ghosts);

	m_inputManager = InputManager::getInstance(m_player);
	m_audioManager = AudioManager::getInstance();	

	m_audioManager->playIntroSound();

	m_astar = AStar();

	//m_astar.findShortestPath(m_graph->getNodeVector()[822], m_graph->getNodeVector()[239], m_graph->getNodeVector()[791]);
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

	switch (globalGameState) {
	
	case GameState::paused:
		onPausedGameState();			
		break;
	case GameState::running:
		onRunningGameState();
		break;
	case GameState::game_over:
		onGameOverGameState();
		break;
	case GameState::life_lost:
		onLifeLostGameState();
		break;	
	}	
}

void GameWorld::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	m_background->render();

	for (auto& dot : m_dots) {
		dot->renderWireframe();
		dot->render();
	}	

	//m_astar.render();

	m_clyde->render();
	m_inky->render();
	m_pinky->render();
	m_blinky->render();

	m_player->render();

	//m_cherry->render();

	renderWireframe();
	renderUi();

	//std::cout << "Pinky current state: " << static_cast<int>(m_pinky->getCurrentState()) << std::endl;
	//std::cout << "Inky current nodfe: " << static_cast<int>(m_inky->getCurrentState()) << std::endl;
	//std::cout << "Current GLOBAL state: " << static_cast<int>(globalGameState) << std::endl;

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

	/*m_blinky->setCurrentDirection(Direction::left);
	m_pinky->setCurrentDirection(Direction::up);
	m_inky->setCurrentDirection(Direction::down);
	m_clyde->setCurrentDirection(Direction::down);*/

	/*m_blinky->changeEnemyState(EnemyState::scatter);
	m_pinky->changeEnemyState(EnemyState::base);
	m_inky->changeEnemyState(EnemyState::base);
	m_clyde->changeEnemyState(EnemyState::base);*/

	m_blinky->restart(blinkyStartNodeIndex, Direction::left);
	m_pinky->restart(pinkyStartNodeIndex, Direction::up);
	m_inky->restart(inkyStartNodeIndex, Direction::down);
	m_clyde->restart(clydeStartNodeIndex, Direction::down);

	m_player->restart(playerStartNodeIndex, Direction::left);		

	frightenedTimer = 0.0f;
	currentBigDotGhostCounter = 1;
	toggleFrightenedMode = false;
}

void GameWorld::restartGame() {
	restart();
	m_player->restartGame();
}

void GameWorld::manageGhostStates() {
	for (auto& interval : intervals) {
		if (!hasIntervalStateChanged && globalTimer > interval.start && globalTimer <= interval.end) {
			if (globalGhostState != interval.state) {

				hasIntervalStateChanged = true;
				globalGhostState = interval.state;

				for (auto& ghost : m_ghosts)
					if (ghost->getCurrentState() != EnemyState::base)
						ghost->changeEnemyState(interval.state);
			}
		}
		else if (hasIntervalStateChanged && globalTimer > interval.end)
			hasIntervalStateChanged = false;
	}
}

void GameWorld::renderUi() {

	TextRenderer::getInstance()->drawStrokeText((char*) "HIGH SCORE", screenWidth - 200 - 50, screenHeight / 2 + 50, 1, 1, 0);

	char score[50];
	sprintf_s(score, 50, "%i", static_cast<int>(m_player->getScore()));
	TextRenderer::getInstance()->drawStrokeText(score, screenWidth - 200, screenHeight / 2, 1, 1, 0);

	char timer[50];
	sprintf_s(timer, 50, "%.2f", globalTimer);
	TextRenderer::getInstance()->drawStrokeText(timer, screenWidth - 200, screenHeight / 2 - 50, 1, 1, 0);
}

void GameWorld::keyboardSpec(int p_key, int p_x, int p_y) {
	m_inputManager->keyboard(p_key, p_x, p_y);
}

void GameWorld::keyboardSpecUp(int p_key, int p_x, int p_y) {
	m_inputManager->keyboardUp(p_key, p_x, p_y);
}

void GameWorld::keyboard(int p_key, int p_x, int p_y) {
	m_inputManager->keyboard(p_key, p_x, p_y);
}

void GameWorld::keyboardUp(int p_key, int p_x, int p_y) {
	m_inputManager->keyboardUp(p_key, p_x, p_y);
}

void GameWorld::joystick(unsigned int p_buttons, int p_x, int p_y, int p_z) {
	m_inputManager->joystick(p_buttons, p_x, p_y, p_z);
}

void GameWorld::onPausedGameState() {

	if (!AudioManager::getInstance()->isPlaying(AudioManager::getInstance()->m_chIntro)) {

		AudioManager::getInstance()->playSiren1Sound();

		//m_pinky->isInsideBase(false);
		m_pinky->exitBase();

		gameStartTimer = 0.0f;

		globalGameState = GameState::running;
	}
}

void GameWorld::onRunningGameState() {

	if (!toggleFrightenedMode)
		globalTimer += m_deltaTime;

	//m_cherry->update(m_deltaTime);

	m_clyde->update(m_deltaTime);
	m_inky->update(m_deltaTime);
	m_pinky->update(m_deltaTime);
	m_blinky->update(m_deltaTime);

	m_player->update(m_deltaTime);

	m_player->eatDot(m_dots);

	if (dotCounter >= inkyDotExitThreshold) {
		//m_inky->isInsideBase(false);
		m_inky->exitBase();
	}

	if (dotCounter >= clydeDotExitThreshold) {
		//m_clyde->isInsideBase(false);
		m_clyde->exitBase();
	}

	if (toggleFrightenedMode) {
		frightenedTimer += m_deltaTime;
		AudioManager::getInstance()->m_chSiren1->stop();
	}

	if (frightenedTimer > frightenedTimerThreshold) {
		for (auto& ghost : m_ghosts) {
			if (ghost->getCurrentState() == EnemyState::frightened) {
				ghost->reverseDirection();
				ghost->returnPreviousEnemyState();
			}
		}
		if (AudioManager::getInstance()->isPlaying(AudioManager::getInstance()->m_chFrightened)) {
			AudioManager::getInstance()->m_chFrightened->stop();
			AudioManager::getInstance()->playSiren1Sound();
		}

		frightenedTimer = 0.0f;
		currentBigDotGhostCounter = 1;
		toggleFrightenedMode = false;
	}

	if (!toggleFrightenedMode)
		manageGhostStates();

	for (auto& ghost : m_ghosts)
		m_player->onGhostCollision(ghost);

	if (m_player->getHealth() <= 0)
		globalGameState = GameState::game_over;
}

void GameWorld::onGameOverGameState() {

	
	//restart();
}

void GameWorld::onNextLevelGameState() {

	nextLevelDelayTimer += m_deltaTime;

	if (nextLevelDelayTimer > nextLevelDelayTimerThreshold) {
		restartGame();
		nextLevelDelayTimer = 0.0f;
	}
}

void GameWorld::onLifeLostGameState() {

	if (AudioManager::getInstance()->isPlaying(AudioManager::getInstance()->m_chFrightened))
		AudioManager::getInstance()->m_chFrightened->stop();

	if (m_player->isAlive())
		m_player->update(m_deltaTime);

	if (m_player->isDeathAnimationFinished()) {
		m_player->setAnimationDelay(normalAnimationDelay);		
		m_player->isAlive(false);
		m_player->isVisible(false);

		
	}
	if (!m_player->isAlive()) {
		lifeLostDelayTimer += m_deltaTime;

		m_player->isVisible(true);

		if (lifeLostDelayTimer > lifeLostDelayTimerThreshold - 2 && !hasRestarted) {
			restart();

			hasRestarted = true;
		}
	
		if (lifeLostDelayTimer > lifeLostDelayTimerThreshold) {
	
			lifeLostDelayTimer = 0;
			hasRestarted = false;
		
			globalGameState = GameState::running;	

			m_player->isAlive(true);
			return;
		}
	}	
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
	render();
}

void GameWorld::idle() {
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	m_deltaTime = (float) (currentTime - m_previousTime) / 1000;
	m_previousTime = currentTime;

	update(m_deltaTime);

	glutPostRedisplay();
}