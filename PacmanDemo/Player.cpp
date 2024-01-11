#include "Player.h"
#include "Enemy.h"
#include <Windows.h>

Player::Player(Sprite p_sprite) : Entity(p_sprite) {

	m_score = 0;
	m_health = 3;
	m_speed = 88.0f*3.0f;

	m_currentNode = getNodeByIndex(441);
	m_position = m_currentNode->getPosition();
	m_position += Vector2D(nodeSize/2.0f, 0.0f);
	m_isMoving = true;
	m_currentDirection = Direction::left;
	m_desiredDirection = Direction::left;
	m_velocity = Vector2D(-1.0f, 0.0f);

	for (size_t i = 1; i <= 3; i++) {
		auto pacImg = new GameObject(Sprite(pacFilePath, 4, 4));
		pacImg->setPosition(Vector2D(screenWidth - (300 - 30 * i), screenHeight / 2 - 100));
		m_pacLives.push_back(pacImg);
	}
}

void Player::update(float p_deltaTime) {
	Entity::update(p_deltaTime);

	m_nextNode = getNodeByDirectionFromCurrentNode(m_desiredDirection);

	setVelocityByDirection();
	updateDirection();
}

void Player::render() {
	Entity::render();
	for (auto& pacLife : m_pacLives)
		pacLife->render();
}

void Player::renderWireframe() {
	Entity::renderWireframe();

	/*auto node = Graph::getInstance()->getNodeInPlayerDirection(m_currentNode, m_currentDirection);

	if (node) {

		glPointSize(16.0f);
		glBegin(GL_POINTS);
		glColor3f(1.0, 0.0, 1.0);
		glVertex2f(node->getPosition().x, node->getPosition().y);
		glEnd();
	}*/

	/*GraphNode* nextNode = getNodeByDirectionFromCurrentNode(m_currentDirection);

	glPointSize(16.0f);
	glBegin(GL_POINTS);
	glColor3f(1.0, 0.0, 1.0);
	glVertex2f(nextNode->getPosition().x, nextNode->getPosition().y);
	glEnd();

	nextNode = getNodeByDirectionFromCurrentNode(m_desiredDirection);

	glPointSize(16.0f);
	glBegin(GL_POINTS);
	glColor3f(1.0, 1.0, 0.0);
	glVertex2f(nextNode->getPosition().x, nextNode->getPosition().y);
	glEnd();*/

	/*if (toggleWireframe) {
		glBegin(GL_POLYGON);
		glColor3fv(m_wireframeColor.toArray());
		glVertex2f(m_position.x - m_size.x / 2, m_position.y - m_size.y / 2);
		glVertex2f(m_position.x + m_size.x / 2, m_position.y - m_size.y / 2);
		glVertex2f(m_position.x + m_size.x / 2, m_position.y + m_size.y / 2);
		glVertex2f(m_position.x - m_size.x / 2, m_position.y + m_size.y / 2);
		glEnd();
	}*/

	/*glBegin(GL_TRIANGLE_FAN);
	glColor3fv(m_wireframeColor.toArray());
	for (int i = 0; i <= 360; ++i) {
		float theta = 2.0f * PI * float(i) / float(360);
		float x = m_position.x + 15 * cos(theta);
		float y = m_position.y + 15 * sin(theta);
		glVertex2f(x, y);
	}
	glEnd();*/
}

void Player::restart(int p_nodeIndex, Direction p_direction) {
	Entity::restart(p_nodeIndex, p_direction);
	m_position += Vector2D(nodeSize / 2.0f, 0.0f);
}

int Player::getScore(void) const {
	return m_score;
}

int Player::getHealth(void) const {
	return m_health;;
}

void Player::eatDot(std::vector<Dot*>& p_dots, std::vector<Enemy*>& p_ghosts) {
	for (auto it = p_dots.begin(); it != p_dots.end();) {
		if (m_position.distanceTo((*it)->getPosition()) < eatDistanceThreshold) {
			if ((*it)->getType() == DotType::big) {
				frightenedTimer = 0.0f;
				toggleFrightenedMode = true;
				 
				for (auto& ghost : p_ghosts) {
					if (ghost->getCurrentMode() != EnemyState::eaten &&
						ghost->getCurrentMode() != EnemyState::base) {
						ghost->changeEnemyState(EnemyState::frightened);
					}
				}
				AudioManager::getInstance()->playFrightenedSound();
			}
			m_score += (*it)->getValue();

			delete* it;
			it = p_dots.erase(it);
		
			AudioManager::getInstance()->playPacEatSound();					
		}
		else
			++it;
	}
}

void Player::onGhostCollision(Enemy* p_ghost) {
	
	if (m_position.distanceTo(p_ghost->getPosition()) < 16) {

		if (p_ghost->getCurrentMode() == EnemyState::chase || 
			p_ghost->getCurrentMode() == EnemyState::scatter) {

			m_health -= 1;
			m_pacLives.erase(m_pacLives.end() - 1);
						
			AudioManager::getInstance()->playDieSound();

			globalGameState = GameState::life_lost;

			return;

		}
		else if (p_ghost->getCurrentMode() == EnemyState::frightened) {

			m_score += initialGhostEatValue * currentBigDotGhostCounter; // 200 * 1
			currentBigDotGhostCounter*=2;			

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

void Player::onPlayerMovement(int p_key) {

	if (p_key == '2') {
		restart(playerStartNodeIndex, Direction::left);
	}

	if (p_key == GLUT_KEY_RIGHT) { // d
		if (m_desiredDirection == Direction::left)
			 m_currentDirection = Direction::right;
		m_desiredDirection = Direction::right;
	}
	if (p_key == GLUT_KEY_LEFT) { // a
		if (m_desiredDirection == Direction::right)
			 m_currentDirection = Direction::left;
		m_desiredDirection = Direction::left;
	}
	if (p_key == GLUT_KEY_UP) {	// w
		if (m_desiredDirection == Direction::down) 
			 m_currentDirection = Direction::up;
		m_desiredDirection = Direction::up;
	}
	if (p_key == GLUT_KEY_DOWN) { // s
		if (m_desiredDirection == Direction::up)
			 m_currentDirection = Direction::down;
		m_desiredDirection = Direction::down;
	}

	if (p_key == GLUT_KEY_RIGHT) { // d
		if (m_desiredDirection == Direction::left)
			m_currentDirection = Direction::right;
		m_desiredDirection = Direction::right;
	}
}

void Player::setDefaultPosition() {
	m_currentNode = getNodeByIndex(441);
	m_position = m_currentNode->getPosition();
	m_position += Vector2D(nodeSize / 2.0f, 0.0f);
}

void Player::setVelocityByDirection() {

	switch (m_currentDirection) {

	case Direction::up:
		m_velocity = Vector2D(0.0f, 1.0f);
		m_sprite.setCurrentFramesRange(6, 8);
		break;
	case Direction::down:
		m_velocity = Vector2D(0.0f, -1.0f);
		m_sprite.setCurrentFramesRange(9, 11);
		break;
	case Direction::left:
		m_velocity = Vector2D(-1.0f, 0.0f);
		m_sprite.setCurrentFramesRange(3, 5);
		break;
	case Direction::right:
		m_velocity = Vector2D(1.0f, 0.0f);
		m_sprite.setCurrentFramesRange(0, 2);
		break;
	case Direction::none:
		m_velocity = Vector2D(0.0f, 0.0f);
		break;
	}
}

void Player::updateDirection() {

	/*float dist = m_position.distanceToSq(m_currentNode->getPosition());	

	if (dist < pacDirectionChangeDistanceThreshold) {
		if (!isValidDirection()) {
			if (m_currentDirection == m_desiredDirection) {
				m_currentDirection = Direction::none;
			}
			else m_desiredDirection = m_currentDirection;
		}
		else {
			m_currentDirection = m_desiredDirection;
		}
	}*/

	// primi input sa tastature w a s d
	// 

	float dist = m_position.distanceToSq(m_currentNode->getPosition());

	if (dist < pacDirectionChangeDistanceThreshold) {
		if (!isValidDirection()) {
			if (m_currentDirection == m_desiredDirection) {
				m_currentDirection = Direction::none;
			}
			//else m_desiredDirection = m_currentDirection;
		}
		else {
			m_currentDirection = m_desiredDirection;
		}
	}
}