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
		}
		break;

		case GhostType::clyde: {

			float dist = m_position.distanceTo(m_player->getPosition());

			drawRectangle(m_position.x, m_position.y, m_wireframeSize.x, m_wireframeSize.y, clydeR, clydeG, clydeB, GL_QUADS);
			drawPoint(m_currentTargetNode->getPosition().x, m_currentTargetNode->getPosition().y, 24, clydeR, clydeG, clydeB);
			drawCircle(m_player->getPosition().x, m_player->getPosition().y, clyde8NodesDistance, 1.0f, 0.0f, 0.0f);
			drawCircle(m_player->getPosition().x, m_player->getPosition().y, dist, 0.0f, 1.0f, 0.0f);

			drawPoint(m_previousNode->getPosition().x, m_previousNode->getPosition().y, 16, 1.0, 0.0f, inkyB);

			if (m_currentTargetNode)
				drawPoint(m_currentTargetNode->getPosition().x, m_currentTargetNode->getPosition().y, 16, .5, 0.0f, inkyB);
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

void Enemy::restart() {
	Entity::restart();

	m_currentTargetNode = m_currentNode;
	m_scatterNode = getNodeByIndex(m_scatterNodeIndices[0]);
	m_baseNode = getNodeByIndex(m_baseNodeIndices[0]);
	m_eatenNode = getNodeByIndex(respawnNodeIndex);

	m_currentDirection = Direction::right;
	m_desiredDirection = Direction::right;

	m_canGoOutsideBase = false;

	m_inBase = m_ghostType == GhostType::blinky ? false : true;

	m_sprite.setCurrentFrame(m_animRange[0]);	

	if (m_ghostType != GhostType::blinky)
		changeEnemyState(EnemyState::base);
	else
		changeEnemyState(globalGhostState);
}

// pozivanje odgovarajuće funkcije zavisno od m_currentDirection
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

// promena vektora brzine tako da se duh kreće na gore
// i podešavanje odgovarajućih tekstura duha za kretanje na gore
int Enemy::onEnemyMoveUp() {

	if (m_velocity != Vector2D(0.0f, 1.0f)) m_velocity = Vector2D(0.0f, 1.0f);

	if (isFrightened()) return setGhostFrightenedTextures();
	if (m_currentEnemyState == EnemyState::eaten) return setGhostEatenTextures(eatenAnimRange[2]);
	return setGhostDefaultTextures(m_animRange[4], m_animRange[5]);
}

// promena vektora brzine tako da se duh kreće na dole
// i podešavanje odgovarajućih tekstura duha za kretanje na dole
int Enemy::onEnemyMoveDown() {

	if (m_velocity != Vector2D(0.0f, -1.0f)) m_velocity = Vector2D(0.0f, -1.0f);

	if (isFrightened()) return setGhostFrightenedTextures();
	if (m_currentEnemyState == EnemyState::eaten) return setGhostEatenTextures(eatenAnimRange[3]);
	return setGhostDefaultTextures(m_animRange[6], m_animRange[7]);
}

// promena vektora brzine tako da se duh kreće u levo
// i podešavanje odgovarajućih tekstura duha za kretanje u levo
int Enemy::onEnemyMoveLeft() {

	if (m_velocity != Vector2D(-1.0f, 0.0f)) m_velocity = Vector2D(-1.0f, 0.0f);

	if (isFrightened()) return setGhostFrightenedTextures();
	if (m_currentEnemyState == EnemyState::eaten) return setGhostEatenTextures(eatenAnimRange[1]);
	return setGhostDefaultTextures(m_animRange[2], m_animRange[3]);
}

// promena vektora brzine tako da se duh kreće u desno
// i podešavanje odgovarajućih tekstura duha za kretanje u desno
int Enemy::onEnemyMoveRight() {

	if (m_velocity != Vector2D(1.0f, 0.0f)) m_velocity = Vector2D(1.0f, 0.0f);

	if (isFrightened()) return setGhostFrightenedTextures();
	if (m_currentEnemyState == EnemyState::eaten) return setGhostEatenTextures(eatenAnimRange[0]);
	return setGhostDefaultTextures(m_animRange[0], m_animRange[1]);	
}

// zadovoljenim uslovom m_currentDirection postaje m_desiredDirection
void Enemy::updateDirection() {
	// provera se preskace kada je stanje Frightened aktivno
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
	default:
		m_currentDirection = Direction::none;
		break;
	}
}

void Enemy::followPath() {

	if (m_path.empty()) return;

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
		if (node && (node != m_currentNode || node != m_previousNode) && !node->isBaseNode())
			m_currentTargetNode = node;

		//if (pathCompleted()) {
			m_playerNode = m_currentTargetNode;
		//}
	}
		break;
	case GhostType::inky: {

		m_currentTargetNode = m_player->getCurrentNode();

		GraphNode* node = getNodeByTwoTargetsDoubled(m_player->getCurrentNode(), m_blinky->getCurrentNode(), m_player->getCurrentDirection());
		if (node)
			m_currentTargetNode = node;

		if (pathCompleted())
			m_playerNode = m_currentTargetNode;

	}
		break;
	case GhostType::clyde: {

		m_currentTargetNode = m_player->getCurrentNode();

		switchClydeToScatter();

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

		switchClydeToChase();
	}
	followPath();
}

void Enemy::onEaten() {
	setSpeed(eatenSpeed);
	setDesiredDirection(getDirectionByNextNode());	

	if (m_currentNode == getNodeByIndex(baseEntranceNodeIndex))
		m_inBase = true;

	if (canUpdateChaseTarget())
		findShortestPath(m_eatenNode);	

	if (pathCompleted()) {
		m_baseNode = getNodeByIndex(m_baseNodeIndices[2]);
		changeEnemyState(EnemyState::base);			
	}
	followPath();
}

void Enemy::onFrightened() {
	setSpeed(frightenedSpeed);
	if (m_currentNode->isTurn()) {
		if (closeToNode()) {
			if (!m_frightenedDirectionChosen) {
				Vector<Direction> possibleDirections = chooseDirectionWhenFrightened();
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

bool Enemy::setGhostFrightenedTextures() {
	return setCurrentFramesRange(frightenedAnimRange[0], frightenedAnimRange[1]);
}

bool Enemy::setGhostFrightenedFlashTextures() {
	return setCurrentFramesRange(frightenedFlashAnimRange[0], frightenedFlashAnimRange[1]);
}

bool Enemy::setGhostEatenTextures(const int index) {
	return setCurrentFramesRange(index, index);
}

bool Enemy::setGhostDefaultTextures(const int index1, const int index2) {
	return setCurrentFramesRange(index1, index2);
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

	if (m_currentNode == getNodeByIndex(baseEntranceNodeIndex)) {
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
			setGhostFrightenedFlashTextures() :
			setGhostFrightenedTextures();
	}
}

void Enemy::changeEnemyState(EnemyState p_enemyState) {

	if (m_currentEnemyState == p_enemyState)
		return;	

	m_previousEnemyState = m_currentEnemyState;
	m_currentEnemyState = p_enemyState;

	m_path.clear();
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
	if (!m_canGoOutsideBase) {
		m_baseNode = m_initialNode;
		m_canGoOutsideBase = true;
	}
}

void Enemy::checkForPortal() {
	Entity::checkForPortal();
}

void Enemy::setSpeed(const float p_speed) {
	if (m_currentEnemyState == globalGhostState &&
		m_currentNode->isTunnelNode()) {
		Entity::setSpeed(ghostTunnelSpeed);
		return;
	}
	Entity::setSpeed(p_speed);
}

void Enemy::assignBlinkyToInky(Enemy* p_blinky) {
	m_blinky = p_blinky;
}

void Enemy::switchClydeToChase() {
	if (m_ghostType == GhostType::clyde) {
		if (globalGhostState == EnemyState::scatter) return;
		if (distanceTo(m_player->getPosition()) > clyde8NodesDistance &&
			!toggleFrightenedMode) {
			changeEnemyState(EnemyState::chase);
		}
	}
}

void Enemy::switchClydeToScatter() {
	if (!toggleFrightenedMode) {
		if (distanceTo(m_player->getPosition()) <= clyde8NodesDistance) {
			changeEnemyState(EnemyState::scatter);
		}
	}
}

// vraća true ukoliko je rastojanje sredine duha 
// do sredine trenutnog čvora dovoljno malo
bool Enemy::closeToNode() {
	return distanceToSq(m_currentNode->getPosition()) < ghostDirectionChangeDistanceThreshold;
}

bool Enemy::canUpdateChaseTarget() {
	return !distanceToSq(m_nextNode->getPosition()) <= nodeSize;
}

bool Enemy::canCalculateNewDirection() const {

	const bool frightenedModeActiveOnBase = toggleFrightenedMode && m_currentEnemyState == EnemyState::base;
	const bool frightenedModeActiveOnEaten = toggleFrightenedMode && m_currentEnemyState == EnemyState::eaten;

	return (m_currentEnemyState != EnemyState::frightened || frightenedModeActiveOnBase || frightenedModeActiveOnEaten);
}

EnemyState Enemy::getCurrentState() const {
	return m_currentEnemyState;
}

Vector<Direction> Enemy::chooseDirectionWhenFrightened() {
	Vector<Direction> directions;
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