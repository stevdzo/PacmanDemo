#include "Player.h"
#include "Enemy.h"
#include <Windows.h>

Player::Player(Sprite p_sprite) : Entity(p_sprite) {

	m_score = 0;
	m_health = health;
	m_speed = pacSpeed;

	m_currentNode = getNodeByIndex(441);
	m_position = m_currentNode->getPosition();
	m_position += Vector2D(nodeSize/2.0f, 0.0f);
	m_isAlive = true;
	m_isVisible = true;
	m_allDotsEaten = false;
	m_currentDirection = Direction::left;
	m_desiredDirection = Direction::left;
	m_velocity = Vector2D(-1.0f, 0.0f);	

	createUIHealth();
}

void Player::update(float p_deltaTime) {

	Entity::update(p_deltaTime);

	m_nextNode = getNodeByDirectionFromCurrentNode(m_desiredDirection);

	checkGameOver();
	checkGameWon();

	setVelocityByDirection();
	updateDirection();

	checkForPortal();
	manageSpeed();
}

void Player::render() {

	if (m_isVisible)
		Entity::render();

	if (!m_pacLives.empty())
		for (auto& pacLife : m_pacLives)
			pacLife->render();
}

void Player::renderWireframe() {
	//Entity::renderWireframe();

	if (toggleWireframe && m_isVisible) {

		auto desiredDirectionNode = getNodeByDirectionFromCurrentNode(m_desiredDirection);
		auto currentDirectionNode = getNodeByDirectionFromCurrentNode(m_currentDirection);

		if (currentDirectionNode)
			drawPoint(currentDirectionNode->getPosition().x, currentDirectionNode->getPosition().y, 18, 0.0f, 1.0f, 1.0f);

		if (desiredDirectionNode)
			drawPoint(desiredDirectionNode->getPosition().x, desiredDirectionNode->getPosition().y, 14, pacR, pacG, pacB);

		
		drawRectangle(m_position.x, m_position.y, m_wireframeSize.x, m_wireframeSize.y, pacR, pacG, pacB, GL_QUADS);
	}
}

void Player::restart() {
	Entity::restart();

	m_currentNode = getNodeByIndex(playerStartNodeIndex);
	m_position = m_currentNode->getPosition();
	m_position += Vector2D(nodeSize / 2.0f, 0.0f);

	m_isAlive = true;
	m_isVisible = true;	
	m_allDotsEaten = false;

	m_currentDirection = Direction::left;
	m_desiredDirection = Direction::left;
	m_velocity = Vector2D(-1.0f, 0.0f);

	resetAnimation();
}

void Player::restartGame() {
	m_allDotsEaten = false;
	m_health = health;
	m_score = 0.0f;

	createUIHealth();
}

int Player::getScore(void) const {
	return m_score;
}

int Player::getHealth(void) const {
	return m_health;
}
void Player::setGhosts(const std::vector<Enemy*>& p_ghosts) {
	m_ghosts = p_ghosts;
}

void Player::createUIHealth() {
	for (size_t i = 1; i <= health; i++) {
		GameObject* pacImg = new GameObject(Sprite(pacFilePath));
		pacImg->setSize(Vector2D(48.0f, 48.0f));
		pacImg->setPosition(Vector2D(screenWidth / 2 + pacUIOffsetX + (pacUISpacingX * i), screenHeight / 2 - pacUIOffsetY));
		m_pacLives.push_back(pacImg);
	}
}

void Player::onDotCollision(std::vector<Dot*>& p_dots) {
	for (auto it = p_dots.begin(); it != p_dots.end();) {
		if (m_position.distanceTo((*it)->getPosition()) < eatDistanceThreshold) {			
			if ((*it)->getType() == DotType::big) {
				onBigDotEaten();
			}
			m_score += (*it)->getValue();
			dotCounter++;

			delete *it;
			it = p_dots.erase(it);
		
			AudioManager::getInstance()->playPacEatSound();					
		}
		else
			++it;
	}

	if (dotCounter >= maxDots) {
		m_allDotsEaten = true;
		dotCounter = 0;
	}
}

void Player::checkGameOver() {
}

void Player::checkGameWon() {
	if (m_allDotsEaten) {
		m_allDotsEaten = false;
		globalGameState = GameState::next_level;

		if (AudioManager::getInstance()->isPlaying(AudioManager::getInstance()->m_chSiren))
			AudioManager::getInstance()->m_chSiren->stop();

		if (AudioManager::getInstance()->isPlaying(AudioManager::getInstance()->m_chFrightened))
			AudioManager::getInstance()->m_chFrightened->stop();

		if (AudioManager::getInstance()->isPlaying(AudioManager::getInstance()->m_chRetreating))
			AudioManager::getInstance()->m_chRetreating->stop();
	}
}

void Player::onGhostCollision(Enemy* p_ghost) {

	if (m_position.distanceTo(p_ghost->getPosition()) <= ghostCollisionDistanceThreshold) {
		if (p_ghost->getCurrentState() == EnemyState::chase ||
			p_ghost->getCurrentState() == EnemyState::scatter)
			onLifeLost();		
		if (p_ghost->getCurrentState() == EnemyState::frightened)
			onGhostEaten(p_ghost);
	}
}

void Player::onBigDotEaten() {	

	for (auto& ghost : m_ghosts) {
		if (ghost->getCurrentState() != EnemyState::eaten &&
		   (ghost->getCurrentState() != EnemyState::base)) {
			ghost->reverseDirection();
			ghost->changeEnemyState(EnemyState::frightened);
		}
	}
	AudioManager::getInstance()->playFrightenedSound();

	frightenedTimer = 0.0f;
	toggleFrightenedMode = true;
}

void Player::onLifeLost() {

	m_isAlive = false;
	
	setAnimationDelay(deathAnimatonDelay);
	setCurrentFramesRange(2, 13, false);

	if (AudioManager::getInstance()->isPlaying(AudioManager::getInstance()->m_chSiren))
		AudioManager::getInstance()->m_chSiren->stop();

	if (AudioManager::getInstance()->isPlaying(AudioManager::getInstance()->m_chFrightened))
		AudioManager::getInstance()->m_chFrightened->stop();

	if (AudioManager::getInstance()->isPlaying(AudioManager::getInstance()->m_chRetreating))
		AudioManager::getInstance()->m_chRetreating->stop();

	AudioManager::getInstance()->playDieSound();

	/*m_pacLives.erase(m_pacLives.end() - 1);
	m_health = m_pacLives.size();*/

	decreaseHealth();

	frightenedTimer = 0.0f;
	toggleFrightenedMode = false;
	currentBigDotGhostCounter = 1;

	globalGameState = GameState::life_lost;
}

void Player::onGhostEaten(Enemy* p_ghost) {
	
	m_score += initialGhostEatValue * currentBigDotGhostCounter; // 200 * 1
	currentBigDotGhostCounter *= 2;

	AudioManager::getInstance()->playEatGhostSound();
	AudioManager::getInstance()->playRetreatingSound();

	p_ghost->reverseDirection();
	//ghost->returnPreviousEnemyState();
	p_ghost->changeEnemyState(EnemyState::eaten);
}

void Player::onPlayerMovement(int p_key) {

	if (!m_isAlive) return;

	if (globalGameState == GameState::running) {

		if (p_key == '2') {
			restart();
		}

		if (p_key == '5') {
			onBigDotEaten();
		}

		if (p_key == GLUT_KEY_RIGHT) { // d
			if (m_desiredDirection == Direction::left && isValidDirection(m_desiredDirection))
				if (m_currentDirection == Direction::left)
					m_currentDirection = Direction::right;

			m_desiredDirection = Direction::right;
		}
		if (p_key == GLUT_KEY_LEFT) { // a
			 if (m_desiredDirection == Direction::right && isValidDirection(m_desiredDirection) && isValidDirection(m_currentDirection))
				 if (m_currentDirection == Direction::right)
					m_currentDirection = Direction::left;

			m_desiredDirection = Direction::left;
		}
		if (p_key == GLUT_KEY_UP) {	// w
			if (m_desiredDirection == Direction::down && isValidDirection(m_desiredDirection) && isValidDirection(m_currentDirection))
				if (m_currentDirection == Direction::down)
					m_currentDirection = Direction::up;
			m_desiredDirection = Direction::up;
		}
		if (p_key == GLUT_KEY_DOWN) { // s
			if (m_desiredDirection == Direction::up && isValidDirection(m_desiredDirection) && isValidDirection(m_currentDirection))
				if (m_currentDirection == Direction::up)
					m_currentDirection = Direction::down;
			m_desiredDirection = Direction::down;
		}
	}
}

void Player::onPlayerJoystickMovement(int p_x, int p_y, int p_z) {

	if (globalGameState == GameState::running) {

		if (p_x > 900) { // d
			if (m_desiredDirection == Direction::left && isValidDirection(m_desiredDirection) && isValidDirection(m_currentDirection))
				if (m_currentDirection == Direction::left)
					m_currentDirection = Direction::right;
			m_desiredDirection = Direction::right;
		}
		if (p_x < -900) { // a
			if (m_desiredDirection == Direction::right && isValidDirection(m_desiredDirection) && isValidDirection(m_currentDirection))
				if (m_currentDirection == Direction::right)
					m_currentDirection = Direction::left;
			m_desiredDirection = Direction::left;
		}
		if (p_y < -900) {	// w
			if (m_desiredDirection == Direction::down && isValidDirection(m_desiredDirection) && isValidDirection(m_currentDirection))
					if (m_currentDirection == Direction::down)
				m_currentDirection = Direction::up;
			m_desiredDirection = Direction::up;
		}
		if (p_y > 900) { // s
			if (m_desiredDirection == Direction::up && isValidDirection(m_desiredDirection) && isValidDirection(m_currentDirection))
				if (m_currentDirection == Direction::up)
					m_currentDirection = Direction::down;
			m_desiredDirection = Direction::down;
		}
	}
}

void Player::isAlive(bool p_isAlive) {
	m_isAlive = p_isAlive;
}

bool Player::isAlive(void) { 
	return m_isAlive;
}

void Player::isVisible(bool p_isVisible) {
	m_isVisible = p_isVisible;
}

bool Player::isVisible(void) {
	return m_isVisible;
}

bool Player::isDeathAnimationFinished() {
	return getCurrentFrame() != 13;
}

bool Player::gameOver() {
	return m_health == 0;
}

void Player::decreaseHealth() {
	m_pacLives.erase(m_pacLives.end() - 1);
	m_health -= 1;
}

void Player::increaseHealth() {

	int lastIndex = m_pacLives.size();

	auto pacImg = new GameObject(Sprite(pacFilePath));
	pacImg->setPosition(Vector2D(screenWidth - (pacUIOffsetX - 30 * lastIndex), screenHeight / 2 - pacUIOffsetY));
	m_pacLives.push_back(pacImg);

	m_health += 1;
}

void Player::checkForPortal() {
	//Entity::checkForPortal();

	if (m_currentNode->getIndex() == leftPortalIndex && m_currentDirection == Direction::left) {
		m_desiredDirection = Direction::left;
		m_position = getNodeByIndex(rightPortalIndex)->getPosition();
	}
	if (m_currentNode->getIndex() == rightPortalIndex && m_currentDirection == Direction::right) {
		m_desiredDirection = Direction::right;
		m_position = getNodeByIndex(leftPortalIndex)->getPosition();
	}
}

void Player::manageSpeed() {

	if (AudioManager::getInstance()->isPlaying(AudioManager::getInstance()->m_chEat))
		Entity::setSpeed(pacDotSpeed);
	else
		Entity::setSpeed(pacSpeed);
}

void Player::resetAnimation() {
	m_sprite.setCurrentFramesRange(0, 1);
	m_sprite.setCurrentFrame(0);
	m_sprite.setAnimationDelay(normalAnimationDelay);
	m_sprite.isLooped(true);
}

void Player::setDefaultPosition() {
	m_currentNode = getNodeByIndex(441);
	m_position = m_currentNode->getPosition();
	m_position += Vector2D(nodeSize / 2.0f, 0.0f);
}

void Player::setVelocityByDirection() {

	if (!m_isAlive) {
		m_currentDirection = Direction::none;
		m_desiredDirection = Direction::none;
		m_velocity = Vector2D(0.0f, 0.0f);
	}

	switch (m_currentDirection) {

	case Direction::up:
		m_velocity = Vector2D(0.0f, 1.0f);
		//m_sprite.setCurrentFramesRange(6, 8);
		m_sprite.setCurrentFramesRange(28, 29);
		break;
	case Direction::down:
		m_velocity = Vector2D(0.0f, -1.0f);
		//m_sprite.setCurrentFramesRange(9, 11);
		m_sprite.setCurrentFramesRange(42, 43);
		break;
	case Direction::left:
		m_velocity = Vector2D(-1.0f, 0.0f);
		//m_sprite.setCurrentFramesRange(3, 5);
		m_sprite.setCurrentFramesRange(14, 15);
		break;
	case Direction::right:
		m_velocity = Vector2D(1.0f, 0.0f);
		//m_sprite.setCurrentFramesRange(0, 2);
		m_sprite.setCurrentFramesRange(0, 1);
		break;
	case Direction::none:
		m_velocity = Vector2D(0.0f, 0.0f);
		break;
	}
}

void Player::updateDirection() {

	float dist = m_position.distanceToSq(m_currentNode->getPosition());

	if (dist < pacDirectionChangeDistanceThreshold) {

		if (isValidDirection(m_currentDirection) && isValidDirection(m_desiredDirection)) {
			m_currentDirection = m_desiredDirection;
		}
		else if (!isValidDirection(m_currentDirection) && isValidDirection(m_desiredDirection)) {
			m_currentDirection = m_desiredDirection;
		}
		else if (isValidDirection(m_currentDirection) && !isValidDirection(m_desiredDirection)) {
			if (m_currentDirection == m_desiredDirection) {
				m_currentDirection = Direction::none;
				m_desiredDirection = Direction::none;
			}
		}
		else if (!isValidDirection(m_currentDirection) && !isValidDirection(m_desiredDirection)) {
			m_currentDirection = Direction::none;
			m_desiredDirection = Direction::none;
		}
	}
}