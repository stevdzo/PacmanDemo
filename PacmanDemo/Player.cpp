#include "Player.h"

Player::Player() : Entity() {
	m_score = 0;
}

void Player::update(float p_deltaTime) {
	//Entity::update(p_deltaTime);
	m_position = m_currentNode->getPosition();
	//std::cout << m_currentNode->getIndex() << std::endl;
}

void Player::render() {
	Entity::render();
}

void Player::renderWireframe() {
	Entity::renderWireframe();
}

void Player::onPlayerMovement(int p_key) {

	int index = -1;

	if (p_key == 'w' || p_key == 'W') {
		index = Graph::getInstance()->getAdjacentNodeIndex(m_currentNode, Direction::up);
	}
	if (p_key == 's' || p_key == 'S') {
		index = Graph::getInstance()->getAdjacentNodeIndex(m_currentNode, Direction::down);
	}
	if (p_key == 'a' || p_key == 'A') {
		index = Graph::getInstance()->getAdjacentNodeIndex(m_currentNode, Direction::left);
	}
	if (p_key == 'd' || p_key == 'D') {
		index = Graph::getInstance()->getAdjacentNodeIndex(m_currentNode, Direction::right);
	}

	if (index != -1) {
		GraphNode* nextNode = Graph::getInstance()->getMatrixAsVector(Graph::getInstance()->getNodes())[index];
		if (nextNode != nullptr && !nextNode->isObstacle()) {
			m_currentNode = nextNode;
		}
	}
}