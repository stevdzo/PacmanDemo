#include "Player.h"
#include "Enemy.h"
#include <Windows.h>

Player::Player(Sprite p_sprite) : Entity(p_sprite) {

	m_score = 0;
	m_health = 3;
	m_speed = 300.0f;

	m_currentNode = getNodeByIndex(441);
	m_position = m_currentNode->getPosition();
	m_position += Vector2D(nodeSize/2.0f, 0.0f);
	m_isAlive = true;
	m_isVisible = true;
	m_deathAnimationStarted = false;
	m_allDotsEaten = false;
	m_currentDirection = Direction::left;
	m_desiredDirection = Direction::left;
	m_velocity = Vector2D(-1.0f, 0.0f);	

	createUIHealth();
}

void Player::update(float p_deltaTime) {

	Entity::update(p_deltaTime);

	m_nextNode = getNodeByDirectionFromCurrentNode(m_desiredDirection);

	onGameWon();

	setVelocityByDirection();
	updateDirection();
}

void Player::render() {

	if (m_isVisible)
		Entity::render();
	for (auto& pacLife : m_pacLives)
		pacLife->render();
}

void Player::renderWireframe() {
	//Entity::renderWireframe();

	auto desiredDirectionNode = getNodeByDirectionFromCurrentNode(m_desiredDirection);
	auto currentDirectionNode = getNodeByDirectionFromCurrentNode(m_currentDirection);

	if (desiredDirectionNode)
		drawPoint(desiredDirectionNode->getPosition().x, desiredDirectionNode->getPosition().y, 14, pacR, pacG, pacB);

	if (currentDirectionNode)
		drawPoint(currentDirectionNode->getPosition().x, currentDirectionNode->getPosition().y, 14, 0.0f, 1.0f, 1.0f);

	if (toggleWireframe)
		drawRectangle(m_position.x, m_position.y, m_wireframeSize.x, m_wireframeSize.y, pacR, pacG, pacB, GL_QUADS);
}

void Player::restart(int p_nodeIndex, Direction p_direction) {
	Entity::restart(p_nodeIndex, p_direction);
	m_position += Vector2D(nodeSize / 2.0f, 0.0f);
	//m_isAlive = true;
	resetAnimation();
}

void Player::restartGame() {
	m_allDotsEaten = false;
	m_health = 3;
	m_score = 0.0f;

	createUIHealth();
}

int Player::getScore(void) const {
	return m_score;
}

int Player::getHealth(void) const {
	return m_health;
}
void Player::createUIHealth() {
	for (size_t i = 1; i <= 3; i++) {
		auto pacImg = new GameObject(Sprite(pacFilePath));
		pacImg->setPosition(Vector2D(screenWidth - (300 - 30 * i), screenHeight / 2 - 100));
		m_pacLives.push_back(pacImg);
	}
}

void Player::eatDot(std::vector<Dot*>& p_dots, std::vector<Enemy*>& p_ghosts) {
	for (auto it = p_dots.begin(); it != p_dots.end();) {
		if (m_position.distanceTo((*it)->getPosition()) < eatDistanceThreshold) {			
			if ((*it)->getType() == DotType::big) {
				frightenedTimer = 0.0f;
				toggleFrightenedMode = true;
				 
				for (auto& ghost : p_ghosts) {
					if (ghost->getCurrentMode() != EnemyState::eaten &&
						(ghost->getCurrentMode() != EnemyState::base)) {
						ghost->setCurrentDirection(ghost->getOppositeDirection());
						ghost->changeEnemyState(EnemyState::frightened);				
					}
				}
				AudioManager::getInstance()->playFrightenedSound();
			}
			m_score += (*it)->getValue();
			dotCounter++;

			delete* it;
			it = p_dots.erase(it);
		
			AudioManager::getInstance()->playPacEatSound();					
		}
		else
			++it;
	}
	if (p_dots.empty())
		m_allDotsEaten = true;
}

void Player::onGameWon() {
	if (m_allDotsEaten) {	
		globalGameState = GameState::next_level;
	}
}

void Player::onGhostCollision(Enemy* p_ghost) {

	if (m_position.distanceTo(p_ghost->getPosition()) < 24) {

		if (p_ghost->getCurrentMode() == EnemyState::chase ||
			p_ghost->getCurrentMode() == EnemyState::scatter) {

			onLifeLost();

			return;

		}
		else if (p_ghost->getCurrentMode() == EnemyState::frightened) {

			m_score += initialGhostEatValue * currentBigDotGhostCounter; // 200 * 1
			currentBigDotGhostCounter *= 2;

			AudioManager::getInstance()->playEatGhostSound();
			p_ghost->isHeadingToHouse(true);
			p_ghost->isFrightened(false);
			p_ghost->changeEnemyState(EnemyState::eaten);

			return;
		}
		else if (p_ghost->getCurrentMode() == EnemyState::eaten) {
			return;
		}
	}
}

void Player::onLifeLost() {

	globalGameState = GameState::life_lost;

	m_currentDirection = Direction::none;
	m_velocity = Vector2D(0.0f, 0.0f);

	setAnimationDelay(deathAnimatonDelay);
	setCurrentFramesRange(2, 13);

	AudioManager::getInstance()->playDieSound();

	m_health -= 1;
	m_pacLives.erase(m_pacLives.end() - 1);
}

void Player::onPlayerMovement(int p_key) {

	if (!m_isAlive) return;

	if (globalGameState == GameState::running) {

		if (p_key == '2') {
			restart(playerStartNodeIndex, Direction::left);
		}

		if (p_key == GLUT_KEY_RIGHT) { // d
			if (m_desiredDirection == Direction::left && isValidDirection(m_desiredDirection))
				m_currentDirection = Direction::right;
			m_desiredDirection = Direction::right;
		}
		else if (p_key == GLUT_KEY_LEFT) { // a
			if (m_desiredDirection == Direction::right && isValidDirection(m_desiredDirection))
				m_currentDirection = Direction::left;
			m_desiredDirection = Direction::left;
		}
		else if (p_key == GLUT_KEY_UP) {	// w
			if (m_desiredDirection == Direction::down && isValidDirection(m_desiredDirection))
				m_currentDirection = Direction::up;
			m_desiredDirection = Direction::up;
		}
		else if (p_key == GLUT_KEY_DOWN) { // s
			if (m_desiredDirection == Direction::up && isValidDirection(m_desiredDirection))
				m_currentDirection = Direction::down;
			m_desiredDirection = Direction::down;
		}
	}
}

void Player::onPlayerJoystickMovement(int p_x, int p_y, int p_z) {

	if (globalGameState == GameState::running) {

		if (p_x > 900) { // d
			if (m_desiredDirection == Direction::left && isValidDirection(m_desiredDirection))
				m_currentDirection = Direction::right;
			m_desiredDirection = Direction::right;
		}
		if (p_x < -900) { // a
			if (m_desiredDirection == Direction::right && isValidDirection(m_desiredDirection))
				m_currentDirection = Direction::left;
			m_desiredDirection = Direction::left;
		}
		if (p_y < -900) {	// w
			if (m_desiredDirection == Direction::down && isValidDirection(m_desiredDirection))
				m_currentDirection = Direction::up;
			m_desiredDirection = Direction::up;
		}
		if (p_y > 900) { // s
			if (m_desiredDirection == Direction::up && isValidDirection(m_desiredDirection))
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

void Player::isVisible(bool m_isVisible) {
	m_isVisible = m_isVisible;
}

bool Player::isVisible(void) {
	return m_isVisible;
}

bool Player::isDeathAnimationFinished() {

	bool flag = getCurrentFrame() == 13;
	if (flag) m_sprite.setCurrentFrame(13);

	return flag;
}

void Player::resetAnimation() {
	m_sprite.setCurrentFrame(0);
}

void Player::setDefaultPosition() {
	m_currentNode = getNodeByIndex(441);
	m_position = m_currentNode->getPosition();
	m_position += Vector2D(nodeSize / 2.0f, 0.0f);
}

void Player::setVelocityByDirection() {

	if (!m_deathAnimationStarted) {

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
}

void Player::updateDirection() {

	float dist = m_position.distanceToSq(m_currentNode->getPosition());

	if (dist < pacDirectionChangeDistanceThreshold) {

		if (!isValidDirection(m_currentDirection) && !isValidDirection(m_desiredDirection)) {
			m_currentDirection = Direction::none;
			m_desiredDirection = Direction::none;
		}
		if (!isValidDirection(m_desiredDirection)) {
			if (m_currentDirection == m_desiredDirection) {
				m_currentDirection = Direction::none;
				m_desiredDirection = Direction::none;
			}
			//else m_desiredDirection = m_currentDirection;
		}
		else
			m_currentDirection = m_desiredDirection;
	}
}