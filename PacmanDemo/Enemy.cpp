#include "Enemy.h"

Enemy::Enemy(GhostType p_ghostType, 
			Sprite p_sprite, 
			const int* p_scatterNodeIndices, 
			const int* p_baseNodeIndices, 
			const EnemyState p_initialState,
			Player* p_player) : 
			Entity(p_sprite), 
			m_astar(AStar()),
			m_ghostType(p_ghostType), 
			m_initialEnemyState(p_initialState),
			m_player(p_player)
{
	m_currentNodeIndex = 0;

	for (unsigned int i = 0; i < 3; i++)
		m_scatterNodeIndices[i] = p_scatterNodeIndices[i];

	for (unsigned int i = 0; i < 3; i++)
		m_baseNodeIndices[i] = p_baseNodeIndices[i];

	m_currentTargetNode = m_currentNode;
	m_scatterNode = Graph::getInstance()->getMatrixAsVector(Graph::getInstance()->getNodes())[m_scatterNodeIndices[0]];
	m_baseNode = Graph::getInstance()->getMatrixAsVector(Graph::getInstance()->getNodes())[m_baseNodeIndices[0]];
	m_eatenNode = Graph::getInstance()->getMatrixAsVector(Graph::getInstance()->getNodes())[respawnNodeIndex];

	m_currentDirection = Direction::right;
	m_desiredDirection = Direction::right;		

	m_frightenedDirectionChosen = false;
	m_frightened = false;
	m_insideBase = true;
	m_isEaten = false;

	m_isClydeInRange = false;
}

void Enemy::update(float p_deltaTime) {
	Entity::update(p_deltaTime);
	
	moveEnemy();	

	setVelocityByDirection();
	updateDirection();

	flashOnFrightened(p_deltaTime);
}

void Enemy::render() {
	Entity::render();
}

void Enemy::renderWireframe() {
	renderPath();	

	//if (toggleWireframe) {

		switch (m_ghostType) {


		case GhostType::blinky: {

			drawRectangle(m_position.x, m_position.y, m_wireframeSize.x, m_wireframeSize.y, blinkyR, blinkyG, blinkyB, GL_QUADS);
			drawPoint(m_currentTargetNode->getPosition().x, m_currentTargetNode->getPosition().y, 24, blinkyR, blinkyG, blinkyB);
		}
		break;

		case GhostType::pinky: {

			drawRectangle(m_position.x, m_position.y, m_wireframeSize.x, m_wireframeSize.y, pinkyR, pinkyG, pinkyB, GL_QUADS);
			drawPoint(m_currentTargetNode->getPosition().x, m_currentTargetNode->getPosition().y, 24, pinkyR, pinkyG, pinkyB);

		} 
		break;

		case GhostType::inky: {

			drawRectangle(m_position.x, m_position.y, m_wireframeSize.x, m_wireframeSize.y, inkyR, inkyG, inkyB, GL_QUADS);
			//drawPoint(m_currentTargetNode->getPosition().x, m_currentTargetNode->getPosition().y, 24, inkyR, inkyG, inkyB);
			GraphNode* node = getNodeByTwoTargetsDoubled(m_player->getCurrentNode(), m_blinky->getCurrentNode(), m_player->getCurrentDirection());
			if (node) {
				drawPoint(node->getPosition().x, node->getPosition().y, 24, 0.5f, 0.5f, 1.0f);
				drawLine(m_blinky->getPosition().x, m_blinky->getPosition().y, node->getPosition().x, node->getPosition().y, inkyR, inkyG, inkyB);
			}
			//auto player2NodesFront = getNodeInDirection(m_player->getCurrentNode(), m_player->getCurrentDirection(), 2);
		}
		break;

		case GhostType::clyde: {

			float dist = m_position.distanceTo(m_player->getPosition());

			drawRectangle(m_position.x, m_position.y, m_wireframeSize.x, m_wireframeSize.y, clydeR, clydeG, clydeB, GL_QUADS);
			drawPoint(m_currentTargetNode->getPosition().x, m_currentTargetNode->getPosition().y, 24, clydeR, clydeG, clydeB);
			drawCircle(m_player->getPosition().x, m_player->getPosition().y, clyde8NodesDistance, 1.0f, 0.0f, 0.0f);
			drawCircle(m_player->getPosition().x, m_player->getPosition().y, dist, 0.0f, 1.0f, 0.0f);

		}
		break;
		}

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
	//}

	//Entity::renderWireframe();	
}

void Enemy::renderPath() {
	for (auto& node : m_path) {
		node->renderNodeFromPath();
	}
}

void Enemy::restart(int p_nodeIndex, Direction p_direction) {
	Entity::restart(p_nodeIndex, p_direction);
	changeEnemyState(m_initialEnemyState);	
	m_scatterNode = Graph::getInstance()->getMatrixAsVector(Graph::getInstance()->getNodes())[m_scatterNodeIndices[0]];
	m_baseNode = Graph::getInstance()->getMatrixAsVector(Graph::getInstance()->getNodes())[m_baseNodeIndices[0]];
	m_eatenNode = Graph::getInstance()->getMatrixAsVector(Graph::getInstance()->getNodes())[respawnNodeIndex];
	m_currentTargetNode = m_baseNode;
	m_currentDirection = Direction::right;
	m_desiredDirection = Direction::right;
	m_frightenedDirectionChosen = false;
	m_frightened = false;
	m_isEaten = false;
	if (m_ghostType != GhostType::blinky)
		m_insideBase = true;

	shouldExitBase(true);

	if (m_currentEnemyState == EnemyState::scatter)
		findShortestPath(m_scatterNode);
	if (m_currentEnemyState == EnemyState::base)
		findShortestPath(m_baseNode);
}

void Enemy::setVelocityByDirection() {
	switch (m_currentDirection) {

	case Direction::up:
		m_velocity = Vector2D(0.0f, 1.0f);

		if (m_currentEnemyState == EnemyState::frightened ||
			(m_currentEnemyState == EnemyState::base && toggleFrightenedMode)) setCurrentFramesRange(8, 9);
		else if (m_currentEnemyState == EnemyState::eaten) setCurrentFramesRange(14, 14);
		else setCurrentFramesRange(4, 5);

		break;
	case Direction::down:
		m_velocity = Vector2D(0.0f, -1.0f);

		if (m_currentEnemyState == EnemyState::frightened ||
			(m_currentEnemyState == EnemyState::base && toggleFrightenedMode)) setCurrentFramesRange(8, 9);
		else if (m_currentEnemyState == EnemyState::eaten) setCurrentFramesRange(15, 15);
		else setCurrentFramesRange(6, 7);

		break;
	case Direction::left:
		m_velocity = Vector2D(-1.0f, 0.0f);

		if (m_currentEnemyState == EnemyState::frightened ||
			(m_currentEnemyState == EnemyState::base && toggleFrightenedMode)) setCurrentFramesRange(8, 9);
		else if (m_currentEnemyState == EnemyState::eaten) setCurrentFramesRange(13, 13);
		else setCurrentFramesRange(2, 3);

		break;
	case Direction::right:
		m_velocity = Vector2D(1.0f, 0.0f);

		if (m_currentEnemyState == EnemyState::frightened ||
			(m_currentEnemyState == EnemyState::base && toggleFrightenedMode)) setCurrentFramesRange(8, 9);
		else if (m_currentEnemyState == EnemyState::eaten) setCurrentFramesRange(12, 12);
		else setCurrentFramesRange(0, 1);

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
	if (dist < ghostDirectionChangeDistanceThreshold) {
		if (m_currentEnemyState != EnemyState::frightened) {
			//if (m_currentNode->isIntersection())
				m_currentDirection = m_desiredDirection;
		}
	}
}

void Enemy::setPositionByNode(const int p_index) {
	Entity::setPositionByNode(p_index);
	m_initialNode = getNodeByIndex(p_index);
}

void Enemy::findShortestPath(GraphNode* p_targetNode) {	
	if (p_targetNode)
		m_path = m_astar.findShortestPath(m_currentNode, p_targetNode);
}

void Enemy::moveEnemy() {

	switch (m_currentEnemyState) {

	case EnemyState::none:
		m_currentDirection = Direction::none;
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
		if (node) {
			m_inkyCurrentTargetNode = node;
			
			std::cout << m_inkyCurrentTargetNode->getIndex() << std::endl;
		}
		m_playerNode = m_inkyCurrentTargetNode;
	}
		break;
	case GhostType::clyde:

		if (m_currentEnemyState != EnemyState::base) {
			if (!toggleFrightenedMode) {
				if (m_position.distanceTo(m_player->getPosition()) <= clyde8NodesDistance) {

					m_isClydeInRange = true;

					changeEnemyState(EnemyState::scatter);
					//std::cout << "scatter" << std::endl;

				}
				else {

					m_isClydeInRange = false;

					changeEnemyState(EnemyState::chase);
					m_playerNode = m_player->getCurrentNode();

					//std::cout << "chase" << std::endl;
				}
			}
		}
		break;
	}
}

void Enemy::onChase() {
	updateChaseTarget();
	m_speed = chaseScatterSpeed;
	m_desiredDirection = getDirectionByNextNode();

	if (onPlayerNodeChange() || m_currentNode->isIntersection()) {
		findShortestPath(m_playerNode); 
		m_currentTargetNode = m_playerNode;
	}
	followPath();	
}

void Enemy::onScatter() {	
	m_speed = chaseScatterSpeed;
	m_desiredDirection = getDirectionByNextNode();
	
	if (pathCompleted()) {
		findShortestPath(m_scatterNode); 
		m_currentTargetNode = m_scatterNode;
		toggleScatterNode();
	}
	followPath();
}

void Enemy::onEaten() {
	m_speed = eatenSpeed;
	m_desiredDirection = getDirectionByNextNode();
	findShortestPath(m_eatenNode);
	m_currentTargetNode = m_eatenNode;
	followPath();

	if (m_currentNode == m_eatenNode) {
		changeEnemyState(EnemyState::scatter);
		m_isEaten = false; 		
	}
}

void Enemy::onFrightened() {		

	m_speed = frightenedSpeed;
	if (m_currentNode->isIntersection()) {		
		if (m_position.distanceTo(m_currentNode->getPosition()) <= ghostDirectionChangeDistanceThreshold) {
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
		m_currentTargetNode = m_baseNode;
		toggleBaseNode();
	}
	followPath();
}

bool Enemy::onEnemyNodeChange() {
	return (m_currentNode == m_nextNode);
	//return (m_currentNode != m_previousNode);
}

bool Enemy::onPlayerNodeChange() {
	return m_player->getCurrentNode() != m_player->getPreviousNode();
}

bool Enemy::pathCompleted() {
	return m_insideBase ? m_path.size() < 1 : m_path.size() < 2;
	//return m_path.size() < 1;

	//if (m_ghostType == GhostType::blinky) 
	//	return m_path.size() < 2;

	//if (!m_insideBase)
	//	return m_path.size() < 2;

	//if (m_insideBase)
	//	if (m_baseNode == getNodeByIndex(baseEntranceNodeIndex))
	//			return m_path.size() < 2;
	//	else return m_path.size() < 1;
}

bool Enemy::isFrightened(void) {
	return m_frightened;
}

void Enemy::isFrightened(bool p_frightened) {
	m_frightened = p_frightened;
}

bool Enemy::isHeadingToHouse(void) {
	return m_isEaten;
}

void Enemy::isHeadingToHouse(bool p_isEaten) {
	m_isEaten = p_isEaten;
}

GraphNode* Enemy::getCurrentTargetNode() const {
	return m_currentTargetNode;
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

	if (m_insideBase) {
		if (m_baseNode == getNodeByIndex(m_baseNodeIndices[0])) {
			m_baseNode = getNodeByIndex(m_baseNodeIndices[1]);
		}
		else if (m_baseNode == getNodeByIndex(m_baseNodeIndices[1])) {
			m_baseNode = getNodeByIndex(m_baseNodeIndices[0]);
		}		
	}
	else {

		if (m_ghostType != GhostType::pinky)
			m_baseNode = m_initialNode;
		else m_baseNode = getNodeByIndex(m_baseNodeIndices[2]);

		//if (m_baseNode == getNodeByIndex(m_baseNodeIndices[0])) {
			//m_baseNode = getNodeByIndex(m_baseNodeIndices[2]);
		//}

			if (m_currentNode == m_baseNode) {
				changeEnemyState(EnemyState::scatter);
				if (toggleFrightenedMode)
					changeEnemyState(EnemyState::frightened);
			}
	}
}

// Flashes the ghosts to white as a sign that the effect is about to end.
void Enemy::flashOnFrightened(float p_deltaTime) {

	if (m_currentEnemyState == EnemyState::frightened  && 
		frightenedTimer > frightenedFlashTimerThreshold) {
		((int)(frightenedTimer * 10) % 3 == 0) ?
			setCurrentFramesRange(10, 11):
			setCurrentFramesRange(8, 9);
	}
}

void Enemy::shouldExitBase(const bool p_insideBase) {
	m_insideBase = !p_insideBase;
}

void Enemy::manageStates() {
	for (const auto& interval : intervals) {
		if (globalTimer > interval.start && globalTimer <= interval.end) {
			if (!m_isEaten) {
				changeEnemyState(interval.state);
			}
		}
	}
}

void Enemy::changeEnemyState(EnemyState p_enemyState) {
	if (m_currentEnemyState != p_enemyState) {

		//std::cout << "dsa" << std::endl;

		/*if (m_previousEnemyState != EnemyState::chase && m_previousEnemyState != EnemyState::scatter) {
			m_previousEnemyState = EnemyState::scatter;
		}*/

		m_previousEnemyState = m_currentEnemyState;
		m_currentEnemyState = p_enemyState;

		if (p_enemyState == EnemyState::chase) {
			updateChaseTarget();
			findShortestPath(m_playerNode);
		}

		/*if (p_enemyState == EnemyState::scatter) {
			findShortestPath(m_scatterNode);
		}*/
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