#include "Enemy.h"

Enemy::Enemy() : Entity() {
}

void Enemy::update(float p_deltaTime) {
	Entity::update(p_deltaTime);
}

void Enemy::render() {
	Entity::render();
}

void Enemy::renderWireframe() {
	m_astar.render();
	Entity::renderWireframe();
	
}

void Enemy::findShortestPath(GraphNode* p_targetNode) {
	m_path = m_astar.findShortestPath(m_currentNode, p_targetNode);
	std::cout << m_path.size() << std::endl;
}