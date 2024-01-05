#include "Enemy.h"

Enemy::Enemy(GhostType p_ghostType, 
			Sprite p_sprite, 
			const int* p_scatterNodeIndices, 
			const int* p_baseNodeIndices, 
			Player* p_player) : 
			Entity(p_sprite), 
			m_ghostType(p_ghostType), 
			m_player(p_player)
{
	m_currentNodeIndex = 0;
	m_speed = 88.0f * 3.0f;

	m_astar = AStar();		
	
	for (unsigned int i = 0; i < 3; i++)
		m_scatterNodeIndices[i] = p_scatterNodeIndices[i];

	for (unsigned int i = 0; i < 3; i++)
		m_baseNodeIndices[i] = p_baseNodeIndices[i];

	m_scatterNode = Graph::getInstance()->getMatrixAsVector(Graph::getInstance()->getNodes())[m_scatterNodeIndices[0]];
	m_baseNode = Graph::getInstance()->getMatrixAsVector(Graph::getInstance()->getNodes())[m_baseNodeIndices[0]];
	m_eatenNode = Graph::getInstance()->getMatrixAsVector(Graph::getInstance()->getNodes())[453];

	m_currentDirection = Direction::right;
	m_desiredDirection = Direction::right;		

	m_frightenedDirectionChosen = false;
	m_insideBase = true;
}

void Enemy::update(float p_deltaTime) {
	Entity::update(p_deltaTime);	

	moveEnemy();	

	setVelocityByDirection();
	updateDirection();
}

void Enemy::render() {
	Entity::render();
}

void Enemy::renderWireframe() {
	renderPath();

	/*if (m_ghostType == GhostType::inky) {

		auto nextNode = getNodeByTwoTargetsDoubled(m_player->getCurrentNode(), m_blinky->getCurrentNode(), m_player->getCurrentDirection());

		if (nextNode) {

			glPointSize(16.0f);
			glBegin(GL_POINTS);
			glColor3f(1.0, 1.0, 0.0);
			glVertex2f(nextNode->getPosition().x, nextNode->getPosition().y);
			glEnd();
		}

	}*/
	/*GraphNode* nextNode = getNodeByDirectionFromCurrentNode(m_desiredDirection);

	glPointSize(16.0f);
	glBegin(GL_POINTS);
	glColor3f(1.0, 1.0, 0.0);
	glVertex2f(nextNode->getPosition().x, nextNode->getPosition().y);
	glEnd();

	nextNode = getNodeByDirectionFromCurrentNode(m_currentDirection);

	glPointSize(16.0f);
	glBegin(GL_POINTS);
	glColor3f(1.0, 0.0, 1.0);
	glVertex2f(nextNode->getPosition().x, nextNode->getPosition().y);
	glEnd();*/

	Entity::renderWireframe();	
}

void Enemy::renderPath() {
	for (auto& node : m_path) {
		node->renderNodeFromPath();
	}
}

void Enemy::setVelocityByDirection() {
	switch (m_currentDirection) {

	case Direction::up:
		m_velocity = Vector2D(0.0f, 1.0f);
		m_currentEnemyState == EnemyState::frightened ? m_sprite.setCurrentFramesRange(8, 9) :
			(m_currentEnemyState == EnemyState::eaten ? m_sprite.setCurrentFramesRange(14, 14) : m_sprite.setCurrentFramesRange(4, 5));
		break;
	case Direction::down:
		m_velocity = Vector2D(0.0f, -1.0f);
		m_currentEnemyState == EnemyState::frightened ? m_sprite.setCurrentFramesRange(8, 9) :
			(m_currentEnemyState == EnemyState::eaten ? m_sprite.setCurrentFramesRange(15, 15) : m_sprite.setCurrentFramesRange(6, 7));
		break;
	case Direction::left:
		m_velocity = Vector2D(-1.0f, 0.0f);
		m_currentEnemyState == EnemyState::frightened ? m_sprite.setCurrentFramesRange(8, 9) :
			(m_currentEnemyState == EnemyState::eaten ? m_sprite.setCurrentFramesRange(12, 12) : m_sprite.setCurrentFramesRange(2, 3));
		break;
	case Direction::right:
		m_velocity = Vector2D(1.0f, 0.0f);
		m_currentEnemyState == EnemyState::frightened ? m_sprite.setCurrentFramesRange(8, 9) :
			(m_currentEnemyState == EnemyState::eaten ? m_sprite.setCurrentFramesRange(11, 11) : m_sprite.setCurrentFramesRange(4, 5));
		break;
	case Direction::none:
		m_velocity = Vector2D(0.0f, 0.0f);
		break;
	}
}

void Enemy::updateDirection() {
	float dist = m_position.distanceToSq(m_currentNode->getPosition());

	if (m_currentEnemyState == EnemyState::base) {
		if (dist < 500) {
			m_currentDirection = m_desiredDirection;
		}
	}
	if (dist < directionChangeDistanceThreshold) {
		if (m_currentEnemyState != EnemyState::frightened) {
			//if (m_currentNode->isIntersection())
				m_currentDirection = m_desiredDirection;
		}
	}
}

void Enemy::findShortestPath(GraphNode* p_targetNode) {	
	if (p_targetNode)
	m_path = m_astar.findShortestPath(m_currentNode, p_targetNode);
}

void Enemy::moveEnemy() {

	switch (m_currentEnemyState) {

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
	case EnemyState::base:
		onBase();
		break;
	}
}

void Enemy::followPath() {

	if (m_path.empty())
		return;

	if (onEnemyNodeChange())
		m_path.erase(m_path.begin());

	m_nextNode = *m_path.begin();
}

void Enemy::updateChaseTarget() {

	switch (m_ghostType) {

	case GhostType::blinky:

		m_playerNode = m_player->getCurrentNode();

		break;
	case GhostType::pinky: {

		GraphNode* node = getNodeInDirection(m_player->getCurrentNode(), m_player->getCurrentDirection(), pinkyTargetNodeDistance);
		if (node)
			m_playerNode = node;
		else
			m_playerNode = m_player->getCurrentNode();
	}
	break;
	case GhostType::inky: {

		GraphNode* node = getNodeByTwoTargetsDoubled(m_player->getCurrentNode(), m_blinky->getCurrentNode(), m_player->getCurrentDirection());
		if (node)
			m_playerNode = node;
		else
			m_playerNode = m_player->getCurrentNode();
	}
		break;
	case GhostType::clyde:

		if (m_currentEnemyState != EnemyState::base) {
			if (getDistanceInNodes(m_player->getCurrentNode()) <= clydeRadiusNodeDistance) {
				if (!toggleFrightenedMode)
					if (m_currentEnemyState != EnemyState::scatter)
						changeEnemyState(EnemyState::scatter);
			}
			else {

				if (!toggleFrightenedMode) 
					if (m_currentEnemyState != EnemyState::chase)
					changeEnemyState(EnemyState::chase);
				m_playerNode = m_player->getCurrentNode();
			}		
		}
		break;
	}
}

void Enemy::onChase() {
	updateChaseTarget();
	m_speed = chaseScatterSpeed;
	m_desiredDirection = getDirectionByNextNode();
	if (onPlayerNodeChange()) findShortestPath(m_playerNode);
	followPath();	
}

void Enemy::onScatter() {	
	m_speed = chaseScatterSpeed;
	m_desiredDirection = getDirectionByNextNode();
	
	if (pathCompleted()) {
		
		findShortestPath(m_scatterNode); 
		toggleScatterNode();
	}
	followPath();
}

void Enemy::onEaten() {
	m_speed = eatenSpeed;
	m_desiredDirection = getDirectionByNextNode();
	findShortestPath(m_eatenNode);
	followPath();
}

void Enemy::onFrightened() {		

	m_speed = frightenedSpeed;
	if (m_currentNode->isIntersection()) {		
		if (m_position.distanceTo(m_currentNode->getPosition()) <= directionChangeDistanceThreshold) {
			if (!m_frightenedDirectionChosen) {				
				std::vector<Direction> possibleDirections = chooseDirectionWhenFrightened();
				if (!possibleDirections.empty()) {
					int randomDirection = rand() % possibleDirections.size();
					Direction newDirection = possibleDirections[randomDirection];
					m_desiredDirection = newDirection;
					m_frightenedDirectionChosen = true;
				
					m_currentDirection = m_desiredDirection;
					//return;
				}
			}
		}
	}		
	m_frightenedDirectionChosen = false;
	m_nextNode = getNodeByDirectionFromCurrentNode(m_desiredDirection);
}

void Enemy::onBase() {
	m_speed = frightenedSpeed;
	m_desiredDirection = getDirectionByNextNode();
	if (pathCompleted()) {
		findShortestPath(m_baseNode);
		toggleBaseNode();
	}
	followPath();
}

bool Enemy::onEnemyNodeChange() {
	return (m_currentNode == m_nextNode);
}

bool Enemy::onPlayerNodeChange() {
	return m_player->getCurrentNode() != m_player->getPreviousNode();
}

bool Enemy::pathCompleted() {
	return m_insideBase ? m_path.size() < 1 : m_path.size() < 2;
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

void Enemy::toggleBaseNode() {

	/*if (m_baseNode == getNodeByIndex(m_baseNodeIndices[0])) {
		m_baseNode = getNodeByIndex(m_baseNodeIndices[1]);
	}
	else if (m_baseNode == getNodeByIndex(m_baseNodeIndices[1])) {
		m_baseNode = getNodeByIndex(m_baseNodeIndices[2]);
	}
	else if (m_baseNode == getNodeByIndex(m_baseNodeIndices[2])) {
		m_baseNode = getNodeByIndex(m_baseNodeIndices[0]);
	}*/

	if (m_insideBase) {
		if (m_baseNode == getNodeByIndex(m_baseNodeIndices[0])) {
			m_baseNode = getNodeByIndex(m_baseNodeIndices[1]);
		}
		else if (m_baseNode == getNodeByIndex(m_baseNodeIndices[1])) {
			m_baseNode = getNodeByIndex(m_baseNodeIndices[0]);
		}		
	}
	else {

		//if (m_baseNode == getNodeByIndex(m_baseNodeIndices[0])) {
			m_baseNode = getNodeByIndex(m_baseNodeIndices[2]);
		//}

		if (m_currentNode == m_baseNode)
			changeEnemyState(EnemyState::scatter);
	}
}

void Enemy::shouldExitBase(const bool p_insideBase) {
	m_insideBase = !p_insideBase;
}

void Enemy::changeEnemyState(EnemyState p_enemyState) {
	m_previousEnemyState = m_currentEnemyState;
	m_currentEnemyState = p_enemyState;

	if (p_enemyState == EnemyState::chase) {
		
		updateChaseTarget();
		findShortestPath(m_playerNode);
	}
}

void Enemy::returnPreviousEnemyState() {
	m_currentEnemyState = m_previousEnemyState;
}

void Enemy::assignBlinkyToInky(Enemy* p_blinky) {
	m_blinky = p_blinky;
}

EnemyState Enemy::getCurrentMode() const {
	return m_currentEnemyState;
}

std::vector<Direction> Enemy::chooseDirectionWhenFrightened() {
	std::vector<Direction> directions;
	m_previousDirection = m_currentDirection;
	if (m_currentNode->isIntersection()) {
		for (auto& node : m_currentNode->getConnectedNodes()) {
			if (!node->isObstacle() && !node->isEmptyNode()) {
				Direction desiredDirection = getDirectionByGivenNode(node);		
				if (!isOppositeDirection(m_previousDirection, desiredDirection)) {									
					directions.push_back(desiredDirection);
				}
			}
		}
	}
	return directions;
}