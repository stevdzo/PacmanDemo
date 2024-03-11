#include "Enemy.h"

Enemy::Enemy(GhostType p_ghostType, 
			Sprite p_sprite, 
			const int* p_scatterNodeIndices, 
			const int* p_baseNodeIndices, 
			const int* p_animRange,
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

	for (unsigned int i = 0; i < 8; i++)
		m_animRange[i] = p_animRange[i];

	m_currentTargetNode = m_currentNode;
	m_scatterNode = Graph::getInstance()->getNodeVector()[m_scatterNodeIndices[0]];
	m_baseNode = Graph::getInstance()->getNodeVector()[m_baseNodeIndices[0]];
	m_eatenNode = Graph::getInstance()->getNodeVector()[respawnNodeIndex];

	m_currentDirection = Direction::right;
	m_desiredDirection = Direction::right;		

	m_frightenedDirectionChosen = false;
	m_frightened = false;
	m_isClydeInRange = false;
	m_isEaten = false;
	m_hasGlobalStateChanged = false;
	m_clydeSwitchState = false;
	m_inkySwitchState = false;

	m_nextClydeState = EnemyState::scatter;
	m_nextInkyState = EnemyState::scatter;

	m_insideBase = m_ghostType == GhostType::blinky ? false : true;

	//m_sprite.setCurrentFramesRange(m_animRange[4], m_animRange[7]);

	m_sprite.setCurrentFrame(m_animRange[0]);

	m_nextNode = m_currentNode;
}

void Enemy::update(float p_deltaTime) {
	Entity::update(p_deltaTime);
	
	moveEnemy();	

	setVelocityByDirection();
	updateDirection();

	flashOnFrightened(p_deltaTime);

	if (m_ghostType == GhostType::clyde)
		shouldClydeSwitchState(p_deltaTime);

	if (m_ghostType == GhostType::inky)
		shouldInkySwitchState(p_deltaTime);
}

void Enemy::render() {
	Entity::render();
}

void Enemy::renderWireframe() {
	//renderPath();	

	if (toggleWireframe) {

		switch (m_ghostType) {


		case GhostType::blinky: {

			drawRectangle(m_position.x, m_position.y, m_wireframeSize.x, m_wireframeSize.y, blinkyR, blinkyG, blinkyB, GL_QUADS);
			//drawPoint(m_currentTargetNode->getPosition().x, m_currentTargetNode->getPosition().y, 24, blinkyR, blinkyG, blinkyB);
		}
		break;

		case GhostType::pinky: {

			drawRectangle(m_position.x, m_position.y, m_wireframeSize.x, m_wireframeSize.y, pinkyR, pinkyG, pinkyB, GL_QUADS);

			/*if (m_currentTargetNode)
				drawPoint(m_currentTargetNode->getPosition().x, m_currentTargetNode->getPosition().y, 24, pinkyR, pinkyG, pinkyB);*/

		} 
		break;

		case GhostType::inky: {

			drawRectangle(m_position.x, m_position.y, m_wireframeSize.x, m_wireframeSize.y, inkyR, inkyG, inkyB, GL_QUADS);
			//drawPoint(m_currentTargetNode->getPosition().x, m_currentTargetNode->getPosition().y, 24, inkyR, inkyG, inkyB);
		/*	GraphNode* node = getNodeByTwoTargetsDoubled(m_player->getCurrentNode(), m_blinky->getCurrentNode(), m_player->getCurrentDirection());
			if (node) {
				drawPoint(node->getPosition().x, node->getPosition().y, 24, inkyR, inkyG, inkyB);
				drawLine(m_blinky->getPosition().x, m_blinky->getPosition().y, node->getPosition().x, node->getPosition().y, inkyR, inkyG, inkyB);
			}

			if (m_playerNode)
				drawPoint(m_playerNode->getPosition().x, m_playerNode->getPosition().y, 24, 1.5f, 1.5f, 1.0f);*/

			//auto player2NodesFront = getNodeInDirection(m_player->getCurrentNode(), m_player->getCurrentDirection(), 2);
		}
		break;

		case GhostType::clyde: {

			//float dist = m_position.distanceTo(m_player->getPosition());

			drawRectangle(m_position.x, m_position.y, m_wireframeSize.x, m_wireframeSize.y, clydeR, clydeG, clydeB, GL_QUADS);
			//drawPoint(m_currentTargetNode->getPosition().x, m_currentTargetNode->getPosition().y, 24, clydeR, clydeG, clydeB);
			//drawCircle(m_player->getPosition().x, m_player->getPosition().y, clyde8NodesDistance, 1.0f, 0.0f, 0.0f);
			//drawCircle(m_player->getPosition().x, m_player->getPosition().y, dist, 0.0f, 1.0f, 0.0f);

		}
		break;
		}
	}

	//Entity::renderWireframe();	
}

void Enemy::renderPath() {
	for (auto& node : m_path) {
		node->renderNodeFromPath();
	}
}

void Enemy::restart(int p_nodeIndex, Direction p_direction) {
	Entity::restart(p_nodeIndex, p_direction);

	if (m_insideBase) changeEnemyState(inkyInitialState);
	else changeEnemyState(globalGhostState);

	m_scatterNode = Graph::getInstance()->getNodeVector()[m_scatterNodeIndices[0]];
	m_baseNode = Graph::getInstance()->getNodeVector()[m_baseNodeIndices[0]];
	m_eatenNode = Graph::getInstance()->getNodeVector()[respawnNodeIndex];
	m_currentTargetNode = m_baseNode;
	m_currentDirection = Direction::right;
	m_desiredDirection = Direction::right;
	m_frightenedDirectionChosen = false;
	m_frightened = false;
	m_isEaten = false;

	if (m_ghostType != GhostType::blinky)
		shouldExitBase(false);
	if (m_ghostType == GhostType::pinky)
		shouldExitBase(true);
	if (m_currentEnemyState == EnemyState::scatter)
		findShortestPath(m_scatterNode);
	if (m_currentEnemyState == EnemyState::base)
		findShortestPath(m_baseNode);

}

void Enemy::setVelocityByDirection() {

	switch (m_currentDirection) {

	case Direction::up:

		onEnemyMoveUp();

		break;
	case Direction::down:

		onEnemyMoveDown();

		break;
	case Direction::left:

		onEnemyMoveLeft();

		break;
	case Direction::right:

		onEnemyMoveRight();

		break;
	case Direction::none:
		m_velocity = Vector2D(0.0f, 0.0f);
		break;
	}
}

void Enemy::onEnemyMoveUp() {

	if (isFrightened()) setCurrentFramesRange(64, 65);
	else if (m_currentEnemyState == EnemyState::eaten) setCurrentFramesRange(80, 80);
	else setCurrentFramesRange(m_animRange[4], m_animRange[5]);

	if (m_velocity == Vector2D(0.0f, 1.0f)) return;

	m_velocity = Vector2D(0.0f, 1.0f);
}

void Enemy::onEnemyMoveDown() {

	if (isFrightened()) setCurrentFramesRange(64, 65);
	else if (m_currentEnemyState == EnemyState::eaten) setCurrentFramesRange(81, 81);
	else setCurrentFramesRange(m_animRange[6], m_animRange[7]);

	if (m_velocity == Vector2D(0.0f, -1.0f)) return;

	m_velocity = Vector2D(0.0f, -1.0f);
}

void Enemy::onEnemyMoveLeft() {

	if (isFrightened()) setCurrentFramesRange(64, 65);
	else if (m_currentEnemyState == EnemyState::eaten) setCurrentFramesRange(79, 79);
	else setCurrentFramesRange(m_animRange[2], m_animRange[3]);

	if (m_velocity == Vector2D(-1.0f, 0.0f)) return;

	m_velocity = Vector2D(-1.0f, 0.0f);
}

void Enemy::onEnemyMoveRight() {

	if (isFrightened()) setCurrentFramesRange(64, 65);
	else if (m_currentEnemyState == EnemyState::eaten) setCurrentFramesRange(78, 78);
	else setCurrentFramesRange(m_animRange[0], m_animRange[1]);

	if (m_velocity == Vector2D(1.0f, 0.0f)) return;

	m_velocity = Vector2D(1.0f, 0.0f);
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
	if (p_targetNode && m_previousNode)
		m_path = m_astar.findShortestPath(m_currentNode, p_targetNode, m_previousNode);
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
		if (node) {
			m_playerNode = node;

			if (m_playerNode == m_currentNode)
				m_playerNode = m_player->getCurrentNode();
		}
	}
		break;
	case GhostType::inky: {

		GraphNode* node = getNodeByTwoTargetsDoubled(m_player->getCurrentNode(), m_blinky->getCurrentNode(), m_player->getCurrentDirection());
		if (node) {
			m_inkyCurrentTargetNode = node;
			m_playerNode = m_inkyCurrentTargetNode;
		}
			
		else if (pathCompleted())
			m_playerNode = m_player->getCurrentNode();

		//
	}
		break;
	case GhostType::clyde:
		if (m_currentEnemyState != EnemyState::base) {
			if (!toggleFrightenedMode) {
				m_playerNode = m_player->getCurrentNode();
				if (m_position.distanceTo(m_player->getPosition()) <= clyde8NodesDistance) {
					m_clydeSwitchState = true;
					m_nextClydeState = EnemyState::scatter;
				}  				
			}
		}
		break;
	}
}

void Enemy::onChase() {
	setSpeed(chaseScatterSpeed);
	setDesiredDirection(getDirectionByNextNode());

	if (m_currentNode->isIntersection() || pathCompleted()) { 
		updateChaseTarget();
		findShortestPath(m_playerNode); 
		m_currentTargetNode = m_playerNode;
	}
	followPath();
}

void Enemy::onScatter() {
	setSpeed(chaseScatterSpeed);
	setDesiredDirection(getDirectionByNextNode());

	findShortestPath(m_scatterNode);
	if (pathCompleted()) {	
		m_currentTargetNode = m_scatterNode;
		toggleScatterNode();
	}
	followPath();

	if (m_ghostType == GhostType::clyde) {
		if (m_position.distanceTo(m_player->getPosition()) > clyde8NodesDistance && 
			globalGhostState == EnemyState::chase) {

			m_clydeSwitchState = true;
			m_nextClydeState = EnemyState::chase;
		}
	}

	if (m_ghostType == GhostType::inky) {

	}
}

void Enemy::onEaten() {
	setSpeed(eatenSpeed);
	setDesiredDirection(getDirectionByNextNode());

	if (pathCompleted()) {

		//std::cout << "11111123213" << std::endl;

		returnPreviousEnemyState();
		m_isEaten = false; 			
	}
	followPath();
}

void Enemy::onFrightened() {		
	setSpeed(frightenedSpeed);
	if (m_position.distanceTo(m_currentNode->getPosition()) <= ghostDirectionChangeDistanceThreshold) {
		if (m_currentNode->isIntersection()) {				
			if (!m_frightenedDirectionChosen) {		
				std::vector<Direction> possibleDirections = chooseDirectionWhenFrightened();
				if (!possibleDirections.empty()) {
					int randomDirection = rand() % possibleDirections.size();
					Direction newDirection = possibleDirections[randomDirection];
					m_desiredDirection = newDirection;		
					m_previousDirection = m_currentDirection;
					m_currentDirection = m_desiredDirection;
					m_frightenedDirectionChosen = true;		

					auto node = getNodeByDirectionFromCurrentNode(m_desiredDirection);
					if (node)
						m_nextNode = node;
				}
			}			
		}
		else m_frightenedDirectionChosen = false;
	}		
}

void Enemy::onBase() {
	setSpeed(baseSpeed);
	setDesiredDirection(getDirectionByNextNode());
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
}

bool Enemy::isFrightened(void) {
	return m_currentEnemyState == EnemyState::frightened ||
		(m_currentEnemyState == EnemyState::base && toggleFrightenedMode);
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

bool Enemy::isInsideBase(void) {
	return m_insideBase;
}

void Enemy::isInsideBase(bool p_insideBase) {
	if (m_insideBase != p_insideBase && !toggleFrightenedMode) m_insideBase = p_insideBase;
}

GhostType Enemy::getGhostType() const {
	return m_ghostType;
}

GraphNode* Enemy::getCurrentTargetNode() const {
	return m_currentTargetNode;
}

void Enemy::setScatterNode(GraphNode* p_node) {
	if (m_scatterNode != p_node)
		m_scatterNode = p_node;
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

		if (m_currentNode == m_baseNode) {


			/*if (getGhostType() == GhostType::inky)
				std::cout << static_cast<int>(getCurrentMode()) << std::endl;*/

			if (toggleFrightenedMode) {
				changeEnemyState(EnemyState::frightened);
				return;
			}

			switch (globalGhostState) {
			case EnemyState::scatter:
				changeEnemyState(EnemyState::scatter);
				break;
			case EnemyState::chase:
				changeEnemyState(EnemyState::chase);
				break;
			}
		}
	}
}

// Flashes the ghosts to white as a sign that the effect is about to end.
void Enemy::flashOnFrightened(float p_deltaTime) {
	if (toggleFrightenedMode)
		if ((m_currentEnemyState == EnemyState::frightened ||
			m_currentEnemyState == EnemyState::base) &&
			frightenedTimer > frightenedFlashTimerThreshold) {
			((int)(frightenedTimer * 10) % 3 == 0) ?
				setCurrentFramesRange(66, 67):
				setCurrentFramesRange(64, 65);
		}
}

void Enemy::shouldExitBase(const bool p_insideBase) {
	if (!toggleFrightenedMode)
		m_insideBase = !p_insideBase;
}

void Enemy::shouldClydeSwitchState(float p_deltaTime) {

	if (m_clydeSwitchState)
		clydeSwitchStateTimer += p_deltaTime;

	if (clydeSwitchStateTimer >= clydeSwitchStateTimerThreshold && !toggleFrightenedMode) {
		if (m_currentNode->isIntersection()) {
			m_clydeSwitchState = false;
			clydeSwitchStateTimer = 0.0f;
			changeEnemyState(m_nextClydeState);
		}
	}
}

void Enemy::shouldInkySwitchState(float p_deltaTime) {
	if (m_inkySwitchState)
		inkySwitchStateTimer += p_deltaTime;

	if (inkySwitchStateTimer >= inkySwitchStateTimerThreshold && !toggleFrightenedMode) {
		if (m_currentNode->isIntersection()) {
			m_inkySwitchState = false;
			inkySwitchStateTimer = 0.0f;
			changeEnemyState(m_nextInkyState);
		}
	}
}

void Enemy::changeEnemyState(EnemyState p_enemyState) {

	if (m_currentEnemyState != p_enemyState) {

		m_previousEnemyState = m_currentEnemyState;
		m_currentEnemyState = p_enemyState;

		if (m_previousEnemyState != EnemyState::chase && 
			m_previousEnemyState != EnemyState::scatter) {
			m_previousEnemyState = globalGhostState;
		}	


		if (p_enemyState == EnemyState::eaten) 
			findShortestPath(m_eatenNode);

		if (p_enemyState == EnemyState::chase) {
			updateChaseTarget();
			findShortestPath(m_playerNode);
		}

		if (p_enemyState == EnemyState::scatter) {
			setScatterNode(getNodeByIndex(m_scatterNodeIndices[0]));
			findShortestPath(m_scatterNode);
		}		
	}
}

void Enemy::returnPreviousEnemyState() {
	m_currentEnemyState = m_previousEnemyState;
}

void Enemy::reverseDirection() {
	if (!m_currentNode->isIntersection() && !toggleFrightenedMode)
		m_currentDirection = getOppositeDirection();
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
			if (!node->isObstacle() && !node->isEmptyNode() && node->getIndex() != baseEntranceBlockNodeIndex) {
				Direction desiredDirection = getDirectionByGivenNode(node);
				if (getOppositeDirection() != desiredDirection) {
					directions.push_back(desiredDirection);
					m_previousDirection = desiredDirection;
				}
			}
		}
	}
	return directions;
}