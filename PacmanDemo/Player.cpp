#include "Player.h"

Player::Player(Sprite p_sprite) : Entity(p_sprite) {

	m_score = 0;
	m_speed = 150.0f;

	m_currentNode = getNodeByIndex(441);
	m_position = m_currentNode->getPosition();
	m_position += Vector2D(nodeSize/2.0f, 0.0f);
	m_isMoving = true;
	m_currentDirection = Direction::left;
	m_desiredDirection = Direction::left;
	m_velocity = Vector2D(-1.0f, 0.0f);
}

void Player::update(float p_deltaTime) {
	Entity::update(p_deltaTime);

	m_nextNode = getNodeByDirectionFromCurrentNode(m_desiredDirection);

	setVelocityByDirection();
	updateDirection();
}

void Player::render() {
	Entity::render();
}

void Player::renderWireframe() {
	Entity::renderWireframe();

	auto node = Graph::getInstance()->getNodeInPlayerDirection(m_currentNode, m_currentDirection);

	if (node) {

		glPointSize(16.0f);
		glBegin(GL_POINTS);
		glColor3f(1.0, 0.0, 1.0);
		glVertex2f(node->getPosition().x, node->getPosition().y);
		glEnd();
	}

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

void Player::eatDot(std::vector<Dot*>& p_dots) {
	for (auto it = p_dots.begin(); it != p_dots.end();) {
		if (m_position.distanceTo((*it)->getPosition()) < eatDistanceThreshold) {
			delete* it;
			it = p_dots.erase(it);
		}
		else ++it;
	}
}

void Player::onPlayerMovement(int p_key) {

	if (p_key == '2') {
		m_currentNode = getNodeByIndex(441);
		m_position = m_currentNode->getPosition();
		m_position += Vector2D(nodeSize / 2.0f, 0.0f);
		m_isMoving = true;
		m_currentDirection = Direction::left;
		m_desiredDirection = Direction::left;
		m_velocity = Vector2D(-1.0f, 0.0f);
	}

	if (p_key == 'd') {
		if (m_desiredDirection == Direction::left) {
			m_desiredDirection = Direction::right;
			m_currentDirection = Direction::right;
		}
		else {
			if (m_currentNode->isIntersection()) {
				if (m_position.distanceTo(m_currentNode->getPosition()) <= turnBufferDistanceThreshold) {
					m_desiredDirection = Direction::right;
				}
				else {					
					/*m_desiredDirection = Direction::left;
					m_currentDirection = Direction::left;*/
				}
			}
		}
	}
	if (p_key == 'a') {
		if (m_desiredDirection == Direction::right) {
			m_desiredDirection = Direction::left;
			m_currentDirection = Direction::left;
		}
		else {
			if (m_currentNode->isIntersection()) {
				if (m_position.distanceTo(m_currentNode->getPosition()) <= turnBufferDistanceThreshold) {
					m_desiredDirection = Direction::left;
				}
				else {
					/*m_desiredDirection = Direction::right;
					m_currentDirection = Direction::right;*/
				}
			}
		}
	}
	if (p_key == 'w') {
		if (m_desiredDirection == Direction::down) {
			m_desiredDirection = Direction::up;
			m_currentDirection = m_desiredDirection;
		}
		else {
			if (m_currentNode->isIntersection()) {
					
				if (m_position.distanceTo(m_currentNode->getPosition()) <= turnBufferDistanceThreshold) {
					m_desiredDirection = Direction::up;
				}
				else {
					/*m_desiredDirection = Direction::down;
					m_currentDirection = Direction::down;*/
				}
			}
		}
	}
	if (p_key == 's') {
		if (m_desiredDirection == Direction::up) {
			m_desiredDirection = Direction::down;
			m_currentDirection = m_desiredDirection;
		}
		else {
			if (m_currentNode->isIntersection()) {
				if (m_position.distanceTo(m_currentNode->getPosition()) <= turnBufferDistanceThreshold) {
					m_desiredDirection = Direction::down;
				}
				else {
					/*m_desiredDirection = Direction::up;
					m_currentDirection = Direction::up;*/
				}
			}
		}
	}
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

	float dist = m_position.distanceToSq(m_currentNode->getPosition());
	if (dist < directionChangeDistanceThreshold) {
		if (!isValidDirection()) {
			if (m_currentDirection == m_desiredDirection) {
				m_currentDirection = Direction::none;
			}
			else m_desiredDirection = m_currentDirection;
		}
		else {
			m_currentDirection = m_desiredDirection;

			//// Snap the position to the current node
			//Vector2D newPosition = m_currentNode->getPosition();
			//m_position = newPosition;

			//// Adjust the position based on the distance
			//float displacement = sqrt(directionChangeDistanceThreshold) - sqrt(dist);
			//m_position += m_velocity * displacement;    
		}
	}
}