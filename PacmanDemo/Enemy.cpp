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
	for (unsigned int i = 0; i < 3; i++)
		m_scatterNodeIndices[i] = p_scatterNodeIndices[i];

	for (unsigned int i = 0; i < 3; i++)
		m_baseNodeIndices[i] = p_baseNodeIndices[i];

	for (unsigned int i = 0; i < 8; i++)
		m_animRange[i] = p_animRange[i];

	m_currentTargetNode = m_currentNode;
	m_scatterNode = getNodeByIndex(m_scatterNodeIndices[0]);
	m_baseNode = getNodeByIndex(m_baseNodeIndices[0]);
	m_eatenNode = getNodeByIndex(respawnNodeIndex);

	m_currentDirection = Direction::right;
	m_desiredDirection = Direction::right;

	m_frightenedDirectionChosen = false;
	m_isEaten = false;

	m_clydeSwitchState = false;
	m_inkySwitchState = false;

	m_nextClydeState = EnemyState::scatter;
	m_nextInkyState = EnemyState::scatter;

	m_inBase = m_ghostType == GhostType::blinky ? false : true;

	m_sprite.setCurrentFrame(m_animRange[0]);

	if (m_ghostType != GhostType::blinky)
		m_nextNode = getNodeByIndex(m_baseNodeIndices[0]);
	else m_nextNode = m_currentNode;
}

void Enemy::update(float p_deltaTime) {

	Entity::update(p_deltaTime);

	updateDirection();

	moveEnemy();	

	setVelocityByDirection();
	
	flashOnFrightened(p_deltaTime);

	if (m_ghostType == GhostType::clyde)
		shouldClydeSwitchState(p_deltaTime);

	checkForPortal();
}

void Enemy::render() {
	Entity::render();
}

void Enemy::renderWireframe() {

	if (toggleWireframe) {

		renderPath();

		switch (m_ghostType) {

		case GhostType::blinky: {

			drawRectangle(m_position.x, m_position.y, m_wireframeSize.x, m_wireframeSize.y, blinkyR, blinkyG, blinkyB, GL_QUADS);
			//drawPoint(m_currentTargetNode->getPosition().x, m_currentTargetNode->getPosition().y, 24, blinkyR, blinkyG, blinkyB);

			drawPoint(m_previousNode->getPosition().x, m_previousNode->getPosition().y, 16, 1.0, 0.0f, inkyB);
		}
		break;

		case GhostType::pinky: {

			drawRectangle(m_position.x, m_position.y, m_wireframeSize.x, m_wireframeSize.y, pinkyR, pinkyG, pinkyB, GL_QUADS);

			//drawPoint(m_currentNode->getPosition().x, m_currentNode->getPosition().y, 16, 1.0, inkyG, inkyB);

			//drawPoint(m_previousNode->getPosition().x, m_previousNode->getPosition().y, 16, 1.0, 0.0f, inkyB);

			//drawPoint(m_nextNode->getPosition().x, m_nextNode->getPosition().y, 16, 1.0, 0.5f, 0.2);

			drawPoint(m_previousNode->getPosition().x, m_previousNode->getPosition().y, 16, 1.0, 0.0f, inkyB);

			if (m_currentTargetNode)
				drawPoint(m_currentTargetNode->getPosition().x, m_currentTargetNode->getPosition().y, 24, pinkyR, pinkyG, pinkyB);
		} 
		break;

		case GhostType::inky: {
		
			drawRectangle(m_position.x, m_position.y, m_wireframeSize.x, m_wireframeSize.y, inkyR, inkyG, inkyB, GL_QUADS);
			drawPoint(m_currentNode->getPosition().x, m_currentNode->getPosition().y, 16, 1.0, inkyG, inkyB);

			drawPoint(m_previousNode->getPosition().x, m_previousNode->getPosition().y, 16, 1.0, 0.0f, inkyB);

			if (m_currentTargetNode)
				drawPoint(m_currentTargetNode->getPosition().x, m_currentTargetNode->getPosition().y, 24, inkyR, inkyG, inkyB);

			if (m_inkyCurrentTargetNode)
				drawPoint(m_inkyCurrentTargetNode->getPosition().x, m_inkyCurrentTargetNode->getPosition().y, 36, inkyR, inkyG, inkyB);
		}
		break;

		case GhostType::clyde: {

			float dist = m_position.distanceTo(m_player->getPosition());

			drawRectangle(m_position.x, m_position.y, m_wireframeSize.x, m_wireframeSize.y, clydeR, clydeG, clydeB, GL_QUADS);
			drawPoint(m_currentTargetNode->getPosition().x, m_currentTargetNode->getPosition().y, 24, clydeR, clydeG, clydeB);
			drawCircle(m_player->getPosition().x, m_player->getPosition().y, clyde8NodesDistance, 1.0f, 0.0f, 0.0f);
			drawCircle(m_player->getPosition().x, m_player->getPosition().y, dist, 0.0f, 1.0f, 0.0f);

			drawPoint(m_previousNode->getPosition().x, m_previousNode->getPosition().y, 16, 1.0, 0.0f, inkyB);
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
	//Entity::restart(p_nodeIndex, p_direction);

	m_currentTargetNode = m_currentNode;
	m_scatterNode = getNodeByIndex(m_scatterNodeIndices[0]);
	m_baseNode = getNodeByIndex(m_baseNodeIndices[0]);
	m_eatenNode = getNodeByIndex(respawnNodeIndex);

	m_currentDirection = Direction::right;
	m_desiredDirection = Direction::right;

	m_clydeSwitchState = false;
	m_inkySwitchState = false;

	m_nextClydeState = EnemyState::scatter;
	m_nextInkyState = EnemyState::scatter;

	m_canGoOutsideBase = false;

	m_inBase = m_ghostType == GhostType::blinky ? false : true;

	m_sprite.setCurrentFrame(m_animRange[0]);

	if (m_ghostType != GhostType::blinky)
		changeEnemyState(EnemyState::base);
	else
		changeEnemyState(globalGhostState);
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
	if (canCalculateNewDirection())
		if (closeToNode())
			m_currentDirection = m_desiredDirection;
}

void Enemy::setPositionByNode(const int p_index) {
	Entity::setPositionByNode(p_index);
	m_initialNode = getNodeByIndex(p_index);
}

void Enemy::findShortestPath(GraphNode* p_targetNode) {

	if (m_currentNode && p_targetNode && m_previousNode)
		m_path = m_astar.findShortestPath(m_currentNode, p_targetNode, m_previousNode, m_currentEnemyState == EnemyState::base);
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

		m_currentTargetNode = m_player->getCurrentNode();
		m_playerNode = m_currentTargetNode;		

		break;
	case GhostType::pinky: {	

		m_currentTargetNode = m_player->getCurrentNode();

		GraphNode* node = getNodeInDirection(m_player->getCurrentNode(), m_player->getCurrentDirection(), pinkyTargetNodeDistance);
		if (node && node != m_currentNode) {
			m_playerNode = node;
		}		

		//if (pathCompleted())
			//m_playerNode = m_currentTargetNode;
	}
		break;
	case GhostType::inky: {

		m_inkyCurrentTargetNode = m_player->getCurrentNode();

		GraphNode* node = getNodeByTwoTargetsDoubled(m_player->getCurrentNode(), m_blinky->getCurrentNode(), m_player->getCurrentDirection());
		if (node)
			m_inkyCurrentTargetNode = node;

		if (pathCompleted())
			m_playerNode = m_inkyCurrentTargetNode;

	}
		break;
	case GhostType::clyde: {

		if (!toggleFrightenedMode) {
			if (m_position.distanceTo(m_player->getPosition()) <= clyde8NodesDistance) {
				m_clydeSwitchState = true;
				m_nextClydeState = EnemyState::scatter;
			}
		}

		if (pathCompleted()) {
			m_currentTargetNode = m_player->getCurrentNode();
			m_playerNode = m_currentTargetNode;
		}
	}
		break;
	}
}

void Enemy::onChase() {
	setSpeed(chaseScatterSpeed);
	setDesiredDirection(getDirectionByNextNode());

	if (canUpdateChaseTarget()) {
		findShortestPath(m_playerNode);
		updateChaseTarget();			
	}
	followPath();
}

void Enemy::onScatter() {
	setSpeed(chaseScatterSpeed);
	setDesiredDirection(getDirectionByNextNode());

	if (canUpdateChaseTarget())
		findShortestPath(m_scatterNode);

	if (pathCompleted()) {	
		m_currentTargetNode = m_scatterNode;
		toggleScatterNode();
	}
	followPath();

	if (m_ghostType == GhostType::clyde) {
		if (m_position.distanceTo(m_player->getPosition()) > clyde8NodesDistance && 
			globalGhostState == EnemyState::chase &&
			!toggleFrightenedMode) {

			m_clydeSwitchState = true;
			m_nextClydeState = EnemyState::chase;
		}
	}
}

void Enemy::onEaten() {
	setSpeed(eatenSpeed);
	setDesiredDirection(getDirectionByNextNode());	

	std::cout << m_inBase << std::endl;

	if (m_currentNode == getNodeByIndex(baseEntranceNodeIndex)) {

		m_inBase = true;
	}

	if (canUpdateChaseTarget()) {
		findShortestPath(m_eatenNode);
	}

	if (pathCompleted()) {

		m_baseNode = getNodeByIndex(m_baseNodeIndices[2]);
		changeEnemyState(EnemyState::base);
		//returnPreviousEnemyState();				
	}
	followPath();
}

void Enemy::onFrightened() {
	setSpeed(frightenedSpeed);
	if (m_currentNode->isTurn()) {
		if (closeToNode()) {
			if (!m_frightenedDirectionChosen) {
				std::vector<Direction> possibleDirections = chooseDirectionWhenFrightened();
				if (!possibleDirections.empty()) {
					int randomDirection = rand() % possibleDirections.size();
					Direction newDirection = possibleDirections[randomDirection];
					m_desiredDirection = newDirection;
					m_previousDirection = m_currentDirection;
					m_currentDirection = m_desiredDirection;
					m_frightenedDirectionChosen = true;

					GraphNode* nextNode = getNodeByDirectionFromCurrentNode(m_desiredDirection);
					if (nextNode)
						m_nextNode = nextNode;
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
		//m_inBase = true;
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
	return m_path.empty();
}

bool Enemy::isFrightened(void) {	

	const bool frightenedModeActiveOnBase = toggleFrightenedMode && m_currentEnemyState == EnemyState::base && m_previousEnemyState != EnemyState::eaten;

	return m_currentEnemyState == EnemyState::frightened || frightenedModeActiveOnBase;
}

void Enemy::isFrightened(bool p_frightened) {
	m_frightened = p_frightened;
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
		return;
	}
	if (m_scatterNode == getNodeByIndex(m_scatterNodeIndices[1])) {
		m_scatterNode = getNodeByIndex(m_scatterNodeIndices[2]);
		return;
	}
	if (m_scatterNode == getNodeByIndex(m_scatterNodeIndices[2])) {
		m_scatterNode = getNodeByIndex(m_scatterNodeIndices[0]);
		return;
	}
}

void Enemy::toggleBaseNode() {

	if (!m_canGoOutsideBase && m_baseNode != getNodeByIndex(m_baseNodeIndices[2])) {
		if (m_baseNode == getNodeByIndex(m_baseNodeIndices[0])) {
			m_baseNode = getNodeByIndex(m_baseNodeIndices[1]);
		}
		else if (m_baseNode == getNodeByIndex(m_baseNodeIndices[1])) {
			m_baseNode = getNodeByIndex(m_baseNodeIndices[0]);
		}
	}

	if (m_canGoOutsideBase && m_baseNode == m_initialNode) {
		m_baseNode = getNodeByIndex(m_baseNodeIndices[2]);
	}

	if (m_previousNode == getNodeByIndex(baseEntranceBlockNodeIndex)) {
		m_inBase = false;

		if (toggleFrightenedMode && m_previousEnemyState != EnemyState::eaten) {
			changeEnemyState(EnemyState::frightened);
			return;
		}
		changeEnemyState(globalGhostState);
	}
}

// Flashes the ghosts to white as a sign that the effect is about to end.
void Enemy::flashOnFrightened(float p_deltaTime) {
	if (isFrightened() &&
		frightenedTimer > frightenedFlashTimerThreshold) {
		((int)(frightenedTimer * 10) % 3 == 0) ?
			setCurrentFramesRange(66, 67) :
			setCurrentFramesRange(64, 65);
	}
}

void Enemy::shouldClydeSwitchState(float p_deltaTime) {

	if (m_clydeSwitchState)
		clydeSwitchStateTimer += p_deltaTime;

	if (clydeSwitchStateTimer >= clydeSwitchStateTimerThreshold && 
		!toggleFrightenedMode && 
		m_currentEnemyState != EnemyState::eaten) {
		if (m_currentNode->isTurn()) {
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
		if (m_currentNode->isTurn()) {
			m_inkySwitchState = false;
			inkySwitchStateTimer = 0.0f;
			changeEnemyState(m_nextInkyState);
		}
	}
}

void Enemy::changeEnemyState(EnemyState p_enemyState) {

	if (m_currentEnemyState == p_enemyState)
		return;

	m_path.clear();

	m_previousEnemyState = m_currentEnemyState;
	m_currentEnemyState = p_enemyState;

	/*if (p_enemyState == EnemyState::base) {
		findShortestPath(m_baseNode);
		return;
	}

	if (p_enemyState == EnemyState::eaten) {
		m_currentTargetNode = m_eatenNode;
		findShortestPath(m_eatenNode);
		return;
	}

	if (p_enemyState == EnemyState::frightened) {		
		return;
	}
	 
	if (p_enemyState == EnemyState::chase) {
		updateChaseTarget();
		findShortestPath(m_playerNode);
		return;
	}

	if (p_enemyState == EnemyState::scatter) {
		setScatterNode(getNodeByIndex(m_scatterNodeIndices[0]));
		findShortestPath(m_scatterNode);
		return;
	}*/
}

void Enemy::returnPreviousEnemyState() {
	changeEnemyState(m_previousEnemyState);
	//m_currentEnemyState = m_previousEnemyState;
}

void Enemy::reverseDirection() {
	if (!closeToNode())
		m_currentDirection = getOppositeDirection();
}

void Enemy::exitBase() {
	if (!m_canGoOutsideBase && !toggleFrightenedMode) {
		m_baseNode = m_initialNode;
		m_canGoOutsideBase = true;
	}
}

void Enemy::checkForPortal() {
	Entity::checkForPortal();
}

void Enemy::assignBlinkyToInky(Enemy* p_blinky) {
	m_blinky = p_blinky;
}

bool Enemy::closeToNode() {
	return m_position.distanceToSq(m_currentNode->getPosition()) < ghostDirectionChangeDistanceThreshold;
}

bool Enemy::canUpdateChaseTarget() {
	return !m_position.distanceToSq(m_nextNode->getPosition()) <= nodeSize;
}

bool Enemy::canCalculateNewDirection() const {

	const bool frightenedModeActiveOnBase = toggleFrightenedMode && m_currentEnemyState == EnemyState::base;
	const bool frightenedModeActiveOnEaten = toggleFrightenedMode && m_currentEnemyState == EnemyState::eaten;

	return (m_currentEnemyState != EnemyState::frightened || frightenedModeActiveOnBase || frightenedModeActiveOnEaten);
}

bool Enemy::canExitFrightened(void) const {
	return m_canExitFrightened;
}

void Enemy::canExitFrightened(bool p_canExitFrightened) {
	m_canExitFrightened = p_canExitFrightened;
}

bool Enemy::allignedWithNode() {
	return false;
}

EnemyState Enemy::getCurrentState() const {
	return m_currentEnemyState;
}

std::vector<Direction> Enemy::chooseDirectionWhenFrightened() {
	std::vector<Direction> directions;
	m_previousDirection = m_currentDirection;
	for (auto& node : m_currentNode->getConnectedNodes()) {
		if (!node->isObstacle() && node->isValidNode() && node->getIndex() != baseEntranceBlockNodeIndex && node->getIndex() != 483) {
			Direction desiredDirection = getDirectionByGivenNode(node);
			if (getOppositeDirection() != desiredDirection) {
				directions.push_back(desiredDirection);
				m_previousDirection = desiredDirection;
			}
		}
	}
	return directions;
}