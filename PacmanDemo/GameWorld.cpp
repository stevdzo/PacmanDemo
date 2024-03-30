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

	m_background->addBgrSprites(Sprite(mazeFlash1FilePath, 1, 1, false));
	m_background->addBgrSprites(Sprite(mazeFlash2FilePath, 1, 1, false));

	m_player = new Player(Sprite(assetsFilePath));

	m_blinky = new Enemy(GhostType::blinky, Sprite(assetsFilePath), blinkyScatterNodeIndices, blinkyBaseNodeIndices, blinkyAnimRange, blinkyInitialState, m_player);
	m_pinky  = new Enemy(GhostType::pinky , Sprite(assetsFilePath), pinkyScatterNodeIndices , pinkyBaseNodeIndices , pinkyAnimRange , pinkyInitialState , m_player);
	m_inky   = new Enemy(GhostType::inky  , Sprite(assetsFilePath), inkyScatterNodeIndices  , inkyBaseNodeIndices  , inkyAnimRange  , inkyInitialState  , m_player);
	m_clyde  = new Enemy(GhostType::clyde , Sprite(assetsFilePath), clydeScatterNodeIndices , clydeBaseNodeIndices , clydeAnimRange , clydeInitialState , m_player);

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
}

void GameWorld::initDots() {

	if (!m_dots.empty()) m_dots.clear();

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
	case GameState::next_level:
		onNextLevelGameState();
		break;
	case GameState::life_lost:
		onLifeLostGameState();
		break;		
	case GameState::intro:
		onIntroGameState();
		break;
	}	
}

void GameWorld::updateGhosts(float p_deltaTime) {
	m_clyde->update(m_deltaTime);
	m_inky->update(m_deltaTime);
	m_pinky->update(m_deltaTime);
	m_blinky->update(m_deltaTime);

	if (dotCounter >= inkyDotExitThreshold)
		m_inky->exitBase();

	if (dotCounter >= clydeDotExitThreshold)
		m_clyde->exitBase();
}

void GameWorld::updatePlayer(float p_deltaTime) {
	m_player->update(m_deltaTime);
}

void GameWorld::render() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	m_background->render();

	for (auto& dot : m_dots) {
		dot->renderWireframe();
		dot->render();
	}

	m_player->render();

	if (globalGameState != GameState::next_level) {
		m_clyde->render();
		m_inky->render();
		m_pinky->render();
		m_blinky->render();
	}

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
}

void GameWorld::gameOver() {
}

void GameWorld::restart() {

	m_blinky->setPositionByNode(blinkyStartNodeIndex);
	m_pinky->setPositionByNode(pinkyStartNodeIndex);
	m_inky->setPositionByNode(inkyStartNodeIndex);
	m_clyde->setPositionByNode(clydeStartNodeIndex);

	m_blinky->restart();
	m_pinky->restart();
	m_inky->restart();
	m_clyde->restart();

	m_player->restart();

	frightenedTimer = 0.0f;
	currentBigDotGhostCounter = 1;
	toggleFrightenedMode = false;
}

void GameWorld::restartGame() {
	restart();
	m_player->restartGame();
}

void GameWorld::manageFrightenedState() {
	if (toggleFrightenedMode) {
		frightenedTimer += m_deltaTime;
		AudioManager::getInstance()->m_chSiren->stop();
	}

	if (frightenedTimer > frightenedTimerThreshold) {
		for (auto& ghost : m_ghosts) {
			if (ghost->getCurrentState() == EnemyState::frightened) {
				ghost->reverseDirection();
				//ghost->returnPreviousEnemyState();
				ghost->changeEnemyState(globalGhostState);
			}
		}
		if (AudioManager::getInstance()->isPlaying(AudioManager::getInstance()->m_chFrightened)) {
			AudioManager::getInstance()->m_chFrightened->stop();
			AudioManager::getInstance()->playSirenSound();
		}

		frightenedTimer = 0.0f;
		currentBigDotGhostCounter = 1;
		toggleFrightenedMode = false;
	}
}

void GameWorld::manageRetreatingSound() {
	if (AudioManager::getInstance()->isPlaying(AudioManager::getInstance()->m_chRetreating)) {
		bool allEaten = std::all_of(m_ghosts.begin(), m_ghosts.end(), [](Enemy*& ghost) {
			return ghost->getCurrentState() != EnemyState::eaten;
		});
		if (allEaten)
			AudioManager::getInstance()->m_chRetreating->stop();
	}
}

void GameWorld::manageSirenSound() {

	int dotCoeff = currentLevel * (2 + currentLevel);

	if (dotCounter == 60 + dotCoeff && !siren2Executed) {
		AudioManager::getInstance()->changeSirenSound();
		chaseScatterSpeed += dotCoeff / 4;
		siren2Executed = true;
	}
	if (dotCounter == 100 + dotCoeff && !siren3Executed) {
		AudioManager::getInstance()->changeSirenSound();
		chaseScatterSpeed += dotCoeff / 4;
		siren3Executed = true;
	}
	if (dotCounter == 140 + dotCoeff && !siren4Executed) {
		AudioManager::getInstance()->changeSirenSound();
		chaseScatterSpeed += dotCoeff / 4;
		siren4Executed = true;
	}
	if (dotCounter == 180 + dotCoeff && !siren5Executed) {
		AudioManager::getInstance()->changeSirenSound();
		chaseScatterSpeed += dotCoeff / 4;
		siren5Executed = true;
	}
}

void GameWorld::manageCollision() {
	for (auto& ghost : m_ghosts)
		m_player->onGhostCollision(ghost);

	m_player->onDotCollision(m_dots);
}

void GameWorld::manageGhostStates() {
	for (auto& interval : intervals) {
		if (!hasIntervalStateChanged && globalTimer > interval.start && globalTimer <= interval.end) {
			if (globalGhostState != interval.state) {

				hasIntervalStateChanged = true;
				globalGhostState = interval.state;

				for (auto& ghost : m_ghosts)
					if (ghost->getCurrentState() != EnemyState::base &&
						ghost->getCurrentState() != EnemyState::eaten) {
						//ghost->reverseDirection();
						ghost->changeEnemyState(interval.state);
					}
			}
		}
		else if (hasIntervalStateChanged && globalTimer > interval.end)
			hasIntervalStateChanged = false;
	}
}

void GameWorld::manageBonusHealth() {
	m_player->increaseHealth();	
}

void GameWorld::adjustLevelStats() {

	currentLevel += 1;

	if (currentLevel > 5) return;

	chaseScatterSpeed = ghostDefaultspeed;

	pacSpeed          += pacSpeed          * speedIncreaseCoeff + currentLevel;
	pacDotSpeed       += pacDotSpeed       * speedIncreaseCoeff + currentLevel;
	ghostTunnelSpeed  += ghostTunnelSpeed  * speedIncreaseCoeff + currentLevel;
	ghostDefaultspeed += ghostDefaultspeed * speedIncreaseCoeff + currentLevel;
	chaseScatterSpeed += ghostDefaultspeed * speedIncreaseCoeff + currentLevel;
	eatenSpeed        += eatenSpeed        * speedIncreaseCoeff + currentLevel;
	frightenedSpeed   += frightenedSpeed   * speedIncreaseCoeff + currentLevel;
	baseSpeed         += baseSpeed         * speedIncreaseCoeff + currentLevel;

	if (frightenedTimerThreshold > 0) {
		frightenedTimerThreshold--;
		frightenedFlashTimerThreshold--;
	}

	if (currentLevel == 5)
		decreaseInterval(intervals);
}

void GameWorld::flashBackgroundOnNextLevel() {
}

void GameWorld::renderUi() {

	int offsetX = 400;
	int offsetY = 100;

	TextRenderer::getInstance()->drawStrokeText((char*) formatElapsedTime(elapsedTime).c_str(), screenWidth - offsetX, screenHeight - offsetY, 1, 1, 0);

	char level[50];
	sprintf_s(level, 50, "Level: %i", currentLevel);
	TextRenderer::getInstance()->drawStrokeText(level, screenWidth - offsetX, screenHeight / 2 + offsetY, 1, 1, 0);

	char dots[50];
	sprintf_s(dots, 50, "Dots eaten: %i", dotCounter);
	TextRenderer::getInstance()->drawStrokeText(dots, screenWidth - offsetX, screenHeight / 2 + offsetY / 2 , 1, 1, 0);

	char score[50];
	sprintf_s(score, 50, "Score: %i", static_cast<int>(m_player->getScore()));
	TextRenderer::getInstance()->drawStrokeText(score, screenWidth - offsetX, screenHeight / 2, 1, 1, 0);

	char timer[50];
	sprintf_s(timer, 50, "Timer: %.2f", globalTimer);
	TextRenderer::getInstance()->drawStrokeText(timer, screenWidth - offsetX, screenHeight / 2 - offsetY/2, 1, 1, 0);

	char* currState = (char*) " ";
	if (globalGhostState == EnemyState::chase)
		currState = (char*) "Chase";
	if (globalGhostState == EnemyState::scatter)
		currState = (char*) "Scatter";

	char state[50];
	sprintf_s(state, 50, "Global state: %s", currState);
	TextRenderer::getInstance()->drawStrokeText(state, screenWidth - offsetX, screenHeight / 2 - offsetY, 1, 1, 0);
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

void GameWorld::onIntroGameState() {

	gameStartTimer += m_deltaTime;

	if (gameStartTimer > gameStartTimerThreshold) {
		gameStartTimer = 0.0f;

		AudioManager::getInstance()->playSirenSound();
		m_pinky->exitBase();
		gameStartTimer = 0.0f;
		globalGameState = GameState::running;
	}
}

void GameWorld::onPausedGameState() {

	gameStartTimer += m_deltaTime;

	if (gameStartTimer > gameRestartTimerThreshold) {

		gameStartTimer = 0.0f;

		m_pinky->exitBase();
		AudioManager::getInstance()->playSirenSound();
		globalGameState = GameState::running;
	}
}

void GameWorld::onRunningGameState() {

	if (!toggleFrightenedMode) {
		globalTimer += m_deltaTime;
		manageGhostStates();
	}

	updateGhosts(m_deltaTime);
	updatePlayer(m_deltaTime);

	manageFrightenedState();
	manageRetreatingSound();
	manageSirenSound();
	manageCollision();	
	manageBonusHealth();
}

void GameWorld::onGameOverGameState() {
	exit(0);
}

void GameWorld::onNextLevelGameState() {

	nextLevelDelayTimer += m_deltaTime;

	m_background->flashBackground(m_deltaTime);

	if (nextLevelDelayTimer > nextLevelDelayTimerThreshold) {

		nextLevelDelayTimer = 0.0f;
		globalTimer = 0.0f;
		hasIntervalStateChanged = false;
		globalGhostState = EnemyState::scatter;	

		initDots();
		restart();

		siren1Executed = false;
		siren2Executed = false;
		siren3Executed = false;
		siren4Executed = false;
		siren5Executed = false;

		currentSirenSoundIndex = 0;

		adjustLevelStats();

		m_background->resetSprite();

		globalGameState = GameState::paused;	
	}
}

void GameWorld::onLifeLostGameState() {

	updatePlayer(m_deltaTime);

	lifeLostDelayTimer += m_deltaTime;

	if (lifeLostDelayTimer > lifeLostDelayTimerThreshold1) {
		m_player->isVisible(false);
		//m_player->isAlive(false);
	}

	if (lifeLostDelayTimer > lifeLostDelayTimerThreshold2) {
		lifeLostDelayTimer = 0.0f;	

		if (m_player->gameOver()) {			
			globalGameState = GameState::game_over;
			return;
		}

		restart();
		globalGameState = GameState::paused;
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

	elapsedTime += m_deltaTime;

	update(m_deltaTime);

	glutPostRedisplay();
}