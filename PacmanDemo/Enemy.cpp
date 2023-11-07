#include "Enemy.h"

Enemy::Enemy(Player* p_player) : m_player(p_player), Entity() {
	m_currentNodeIndex = 0;
	m_velocity = Vector2D();
	m_speed = 140.0f;
	m_enemyState = EnemyState::follow_path;

	m_astar = AStar();

	findShortestPath(Graph::getInstance()->getMatrixAsVector(Graph::getInstance()->getNodes())[866]);
}

void Enemy::update(float p_deltaTime) {
	Entity::update(p_deltaTime);

	moveEnemy();

	updateDirection();
}

void Enemy::render() {
	Entity::render();
}

void Enemy::renderWireframe() {
	renderPath();
	Entity::renderWireframe();	
}

void Enemy::renderPath() {
	for (auto& node : m_path) {
		node->renderNodeFromPath();
	}
}

void Enemy::findShortestPath(GraphNode* p_targetNode) {	
	m_path = m_astar.findShortestPath(m_currentNode, p_targetNode);
}

void Enemy::moveEnemy() {

	m_desiredDirection = getDirectionByNode();

	switch (m_enemyState) {

	case EnemyState::idle:
		m_currentDirection = none;
		break;
	case EnemyState::wander:
		wander();
		break;
	case EnemyState::follow_path:
		followPath();
		break;		
	}
}

void Enemy::wander() {
	if (m_currentNode->isIntersection()) {
		//if (isValidDirection()) {
			int newDir = rand() % 4;
			int index = getNodeIndexByDirection(static_cast<Direction>(newDir));
			m_nextNode = getNodeByIndex(index);
		//}
	}
}

void Enemy::followPath() {

	if (m_path.empty()) {	
		return;
	}
	
	m_nextNode = *m_path.begin();

	if (onEnemyNodeChange()) {
		auto it = m_path.erase(m_path.begin());
	}
	if (onPlayerNodeChange()) {

		findShortestPath(m_player->getCurrentNode());
	}
}

bool Enemy::onEnemyNodeChange() {
	return m_currentNode == m_nextNode;
}

bool Enemy::onPlayerNodeChange() {
	return m_player->getCurrentNode() != m_player->getPreviousNode();
}