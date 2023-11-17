#include "Enemy.h"

Enemy::Enemy(Sprite p_sprite, Player* p_player) : Entity(p_sprite), m_player(p_player)  {
	m_currentNodeIndex = 0;
	m_velocity = Vector2D();
	m_speed = 140.0f;
	m_enemyState = EnemyState::chase;

	m_astar = AStar();

	for (int i = 0; i < 3; i++)
		m_scatterNodeIndices[i] = blinkyScatterNodeIndices[i];	

	m_scatterNode = Graph::getInstance()->getMatrixAsVector(Graph::getInstance()->getNodes())[m_scatterNodeIndices[0]];
	m_eatenNode = Graph::getInstance()->getMatrixAsVector(Graph::getInstance()->getNodes())[453];

	findShortestPath(m_scatterNode);

	m_currentDirection = Direction::right;
	m_desiredDirection = Direction::right;	
}

void Enemy::update(float p_deltaTime) {
	Entity::update(p_deltaTime);

	moveEnemy();	

	Entity::updateDirection();
}

void Enemy::render() {
	Entity::render();
}

void Enemy::renderWireframe() {
	renderPath();

	GraphNode* nextNode = getNodeByDirectionFromCurrentNode(m_currentDirection);

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
	glEnd();

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

	switch (m_enemyState) {

	case EnemyState::none:
		m_currentDirection = none;
		break;
	case EnemyState::chase:
		onChase();
		break;
	case EnemyState::scatter:
		onScatter();
		break;
	case EnemyState::eaten:
		onEaten();
		break;
	case EnemyState::frightened:
		onFrightened();
		break;
	}
}

void Enemy::followPath() {

	if (m_path.empty())
		return;

	m_nextNode = *m_path.begin();

	if (onEnemyNodeChange())
		m_path.erase(m_path.begin());
}

void Enemy::onChase() {
	m_desiredDirection = getDirectionByNextNode();
	if (onPlayerNodeChange()) findShortestPath(m_player->getCurrentNode());
	followPath();
}

void Enemy::onScatter() {
	m_desiredDirection = getDirectionByNextNode();
	if (pathCompleted()) {
		findShortestPath(m_scatterNode);
	}
	toggleScatterNode();
	followPath();
}

void Enemy::onEaten() {
	m_desiredDirection = getDirectionByNextNode();
	findShortestPath(m_eatenNode);
	followPath();
}

void Enemy::onFrightened() {

	std::vector<Direction> possibleDirections = chooseDirectionWhenFrightened();

	if (m_currentNode->isIntersection()) {
		if (!possibleDirections.empty()) {
			int randomDirection = rand() % possibleDirections.size();

			Direction newDirection = possibleDirections[randomDirection];

			m_desiredDirection = newDirection;
		}
	}
	m_nextNode = getNodeByDirectionFromCurrentNode(m_desiredDirection);
}

bool Enemy::onEnemyNodeChange() {
	return m_currentNode == m_nextNode;
}

bool Enemy::onPlayerNodeChange() {
	return m_player->getCurrentNode() != m_player->getPreviousNode();
}

bool Enemy::pathCompleted() {
	return m_path.size() < 1;
}

void Enemy::toggleScatterNode() {

	if (m_scatterNode == getNodeByIndex(m_scatterNodeIndices[0])) {
		m_scatterNode = getNodeByIndex(m_scatterNodeIndices[1]);
	}
	else if (m_scatterNode == getNodeByIndex(m_scatterNodeIndices[1])) {
		m_scatterNode = getNodeByIndex(m_scatterNodeIndices[2]);
	}
	else if (m_scatterNode == getNodeByIndex(m_scatterNodeIndices[2])) {
		m_scatterNode = getNodeByIndex(m_scatterNodeIndices[0]);
	}
}

std::vector<Direction> Enemy::chooseDirectionWhenFrightened() {

	std::vector<Direction> directions;
	if (m_currentNode->isIntersection()) {
		for (auto& node : m_currentNode->getConnectedNodes()) {	
			if (!node->isObstacle() && !node->isEmptyNode()) {
				Direction desiredDirection = getDirectionByGivenNode(node);
				if (!isOppositeDirection(m_currentDirection, desiredDirection)) {
					directions.push_back(desiredDirection);
				}
			}
		}
	}
	return directions;
}