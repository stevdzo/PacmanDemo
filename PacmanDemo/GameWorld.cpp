#include "GameWorld.h"
#include "Graph.h"
#include "InputManager.h"
#include "AudioManager.h"
#include "TextRenderer.h"

void GameWorld::init() {
	std::cout << "***********************************" << std::endl;
	std::cout << "*** OpenGL Pacman demo ~ Stevan ***" << std::endl;
	std::cout << "***********************************" << std::endl;

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // nicijalna boja pozadine

	srand(time(0)); // pseudo random generator 

	initDots(); // inicijalizovanje tačaka

	m_background = new GameObject(Sprite(mazeFilePath, 1, 1, false));  // kreiranje objekta pozadine
	m_background->setPosition(Vector2D((screenWidth-backgroundXOffset) / 2, screenHeight / 2));
	m_background->setSize(Vector2D(screenWidth- backgroundXOffset, screenHeight));

	// dodavanje još dva sprajta objektu pozadine, zbog animacije prelaska na sledeći nivo
	m_background->addBgrSprites(Sprite(mazeFlash1FilePath, 1, 1, false)); 
	m_background->addBgrSprites(Sprite(mazeFlash2FilePath, 1, 1, false));

	m_player = new Player(Sprite(assetsFilePath)); // kreiranje objekta igrača

	// kreiranje objekata duhova
	m_blinky = new Enemy(GhostType::blinky, Sprite(assetsFilePath), blinkyScatterNodeIndices, blinkyBaseNodeIndices, blinkyAnimRange, blinkyInitialState, m_player);
	m_pinky  = new Enemy(GhostType::pinky , Sprite(assetsFilePath), pinkyScatterNodeIndices , pinkyBaseNodeIndices , pinkyAnimRange , pinkyInitialState , m_player);
	m_inky   = new Enemy(GhostType::inky  , Sprite(assetsFilePath), inkyScatterNodeIndices  , inkyBaseNodeIndices  , inkyAnimRange  , inkyInitialState  , m_player);
	m_clyde  = new Enemy(GhostType::clyde , Sprite(assetsFilePath), clydeScatterNodeIndices , clydeBaseNodeIndices , clydeAnimRange , clydeInitialState , m_player);

	// postavljanje duhova na njihovu početnu poziciju
	m_blinky->setPositionByNode(blinkyStartNodeIndex);
	m_pinky->setPositionByNode(pinkyStartNodeIndex);
	m_inky->setPositionByNode(inkyStartNodeIndex);
	m_clyde->setPositionByNode(clydeStartNodeIndex);

	// postavljanje prvobitnog smera kretanja duhova
	m_blinky->setCurrentDirection(Direction::left);
	m_pinky->setCurrentDirection(Direction::down);
	m_inky->setCurrentDirection(Direction::up);
	m_clyde->setCurrentDirection(Direction::down);

	// postavljanje inicijalnog stanja duhova
	m_blinky->changeEnemyState(EnemyState::scatter);
	m_pinky->changeEnemyState(EnemyState::base);
	m_inky->changeEnemyState(EnemyState::base);
	m_clyde->changeEnemyState(EnemyState::base);

	m_inky->assignBlinkyToInky(m_blinky); // dodeljivanje crvenog duha plavom, zbog njegovog ponašanja

	// dodavanje duhova u vektor 
	m_ghosts.push_back(m_blinky);
	m_ghosts.push_back(m_pinky);
	m_ghosts.push_back(m_inky);
	m_ghosts.push_back(m_clyde);

	m_player->setGhosts(m_ghosts); // dodeljivanje duhova igraču

	InputManager::getInstance()->setPlayer(m_player); // dodeljivanje igrača singleton klasi InputManager

	AudioManager::getInstance()->playIntroSound(); // puštanje početne melodije na startu igre
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

void GameWorld::update() {
	switch (globalGameState) {
	
	case GameState::running:
		onRunningGameState();			
		break;
	case GameState::paused:
		onPausedGameState();
		break;
	case GameState::game_over:
		onGameOverGameState();
		break;
	case GameState::life_lost:
		onLifeLostGameState();
		break;
	case GameState::next_level:
		onNextLevelGameState();
		break;			
	case GameState::start:
		onStartGameState();
		break;
	}	
}

void GameWorld::updateGhosts() {
	m_clyde->update(m_deltaTime);
	m_inky->update(m_deltaTime);
	m_pinky->update(m_deltaTime);
	m_blinky->update(m_deltaTime);

	if (dotCounter >= inkyDotExitThreshold)
		m_inky->exitBase();	

	if (dotCounter >= clydeDotExitThreshold)
		m_clyde->exitBase();
}

void GameWorld::updatePlayer() {
	m_player->update(m_deltaTime);
}

void GameWorld::render() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // brisanje buffer-a boje i dubine
	glLoadIdentity(); // resetovanje matrice na jediničnu 

	m_background->render(); // iscrtavanje objekta pozadine

	// iscrtavanje tačaka
	for (auto& dot : m_dots) 	
		dot->render();

	m_player->render(); // iscrtavanje igrača

	// iscrtavanje duhova, ukoliko trenutno stanje igre nije next_level
	if (globalGameState != GameState::next_level) {
		m_clyde->render();
		m_inky->render();
		m_pinky->render();
		m_blinky->render();
	}

	renderWireframe(); // render za mrežni prikaz
	renderUi(); // render UI elemenata

	glutSwapBuffers(); // pražnjenje buffer-a kada je aktivno dvostruko buffer-ovanje
}

void GameWorld::renderWireframe() {
	Graph::getInstance()->renderWireframe();
	for (auto& dot : m_dots)
		dot->renderWireframe();
	m_player->renderWireframe();
	m_clyde->renderWireframe();
	m_inky->renderWireframe();
	m_pinky->renderWireframe();
	m_blinky->renderWireframe();
	
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

	if (toggleFrightenedMode) return; 

	globalTimer += m_deltaTime;
	for (auto& interval : intervals) {
		if (!hasIntervalStateChanged && globalTimer > interval.start && globalTimer <= interval.end) {
			if (globalGhostState != interval.state) {

				hasIntervalStateChanged = true; // provera da se interval menja tačno jednom
				globalGhostState = interval.state; // promena stanja

				// za svakog duha menja se stanje u novo, ukoliko duhovi nisu u bazi ili nisu pojedeni
				for (auto& ghost : m_ghosts)
					if (ghost->getCurrentState() != EnemyState::base &&
						ghost->getCurrentState() != EnemyState::eaten) {
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
	InputManager::getInstance()->keyboardSpec(p_key, p_x, p_y);
}

void GameWorld::keyboardSpecUp(int p_key, int p_x, int p_y) {
	InputManager::getInstance()->keyboardSpecUp(p_key, p_x, p_y);
}

void GameWorld::keyboard(int p_key, int p_x, int p_y) {
	InputManager::getInstance()->keyboard(p_key, p_x, p_y);
}

void GameWorld::keyboardUp(int p_key, int p_x, int p_y) {
	InputManager::getInstance()->keyboardUp(p_key, p_x, p_y);
}

void GameWorld::joystick(unsigned int p_buttons, int p_x, int p_y, int p_z) {
	InputManager::getInstance()->joystick(p_buttons, p_x, p_y, p_z);
}

void GameWorld::onStartGameState() {

	auto chIntro = AudioManager::getInstance()->m_chIntro;

	if (!AudioManager::getInstance()->isPlaying(chIntro)) {
		m_pinky->exitBase();
		AudioManager::getInstance()->playSirenSound();
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
			
	updateGhosts();
	updatePlayer();

	manageGhostStates();
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
		dotCounter = 0;

		adjustLevelStats();

		m_background->resetSprite();

		globalGameState = GameState::paused;	
	}
}

void GameWorld::onLifeLostGameState() {

	updatePlayer();

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
	InputManager::getInstance()->mouse(p_button, p_state, p_x, p_y);
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
	// izračunavanje delta vremena, tj. vremenske razlike između dva frejma
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	m_deltaTime = (float) (currentTime - m_previousTime) / ms;
	m_previousTime = currentTime;

	elapsedTime += m_deltaTime; // tajmer koji računa vreme od pokretanja programa

	update();

	glutPostRedisplay(); // trigger-ovanje display callback-a
}