#include "Entity.h"

int Entity::getDistanceInNodes(GraphNode* m_targetNode) const {
    return Graph::getInstance()->calculateDistanceInNodes(m_currentNode, m_targetNode);
}

// Gets the node positioned next to current node by given direction
int Entity::getNodeIndexByDirection(Direction p_direction) const {
    return Graph::getInstance()->getNodeIndexByDirection(m_currentNode, p_direction);
}

// Gets the node from graph by index
GraphNode* Entity::getNodeByIndex(int p_index) const {
    return Graph::getInstance()->getNodeVector()[p_index];
}

// Gets the node from given direction by current node
GraphNode* Entity::getNodeByDirectionFromCurrentNode(Direction p_direction) const {
    auto index = getNodeIndexByDirection(p_direction);
    auto node = getNodeByIndex(index);
    return node ? node : nullptr;
}

// Gets the node from given direction by custom node
GraphNode* Entity::getNodeByDirectionFromCustomNode(GraphNode* p_node, Direction p_direction) const {
    return getNodeByIndex(Graph::getInstance()->getNodeIndexByDirection(p_node, p_direction));
}

// Gets the node from graph x nodes from current node based on current direction
GraphNode* Entity::getNodeInDirection(GraphNode* p_node, Direction p_direction, const int p_tileDistance) const {
    return Graph::getInstance()->getNodeInPlayerDirection(p_node, p_direction, p_tileDistance);
}

// Gets the node from graph by current position of entity
GraphNode* Entity::getNodeByPosition() const {
    auto node = Graph::getInstance()->getNodeByPosition(m_position);
    return (node != nullptr) ? node : m_previousNode;
}

// Gets the node by given position
GraphNode* Entity::getNodeByPosition(Vector2D p_position) const {
    return Graph::getInstance()->getNodeByPosition(p_position);
}

// Gets the node based on two targets doubling the distance (for inky)
GraphNode* Entity::getNodeByTwoTargetsDoubled(GraphNode* p_node1, GraphNode* p_node2, Direction p_direction) const {
    return Graph::getInstance()->calculateInkyTargetNode(p_node1, p_node2, p_direction);
}

// Gets the direction between current and next node
Direction Entity::getDirectionByNextNode() const {
    return Graph::getInstance()->getDirectionByNode(m_currentNode, m_nextNode);
}

// Gets the direction from current to given node
Direction Entity::getDirectionByGivenNode(GraphNode* p_node) const {
    return Graph::getInstance()->getDirectionByNode(m_currentNode, p_node);
}

void Entity::setVelocityByDirection() {

    switch (m_currentDirection) {

    case Direction::up:
        m_velocity = Vector2D(0.0f, 1.0f);
        break;
    case Direction::down:
        m_velocity = Vector2D(0.0f, -1.0f);
        break;
    case Direction::left:
        m_velocity = Vector2D(-1.0f, 0.0f);
        break;
    case Direction::right:
        m_velocity = Vector2D(1.0f, 0.0f);
        break;
    case Direction::none:
        m_velocity = Vector2D(0.0f, 0.0f);
        break;
    }
}

void Entity::checkForPortal() {
    if (m_currentNode->getIndex() == leftPortalIndex && m_currentDirection == Direction::left)
        m_position = getNodeByIndex(rightPortalIndex)->getPosition();
    if (m_currentNode->getIndex() == rightPortalIndex && m_currentDirection == Direction::right)
        m_position = getNodeByIndex(leftPortalIndex)->getPosition();
}

void Entity::updateDirection() {
     
}

bool Entity::isValidDirection(const Direction p_direction) const {
    auto node = getNodeByDirectionFromCurrentNode(p_direction);
    return !(node && (node->isEmptyNode() || node->isObstacle() || node->getIndex() == baseEntranceBlockNodeIndex));
}

bool Entity::isOppositeDirection(Direction p_direction1, Direction p_direction2) const {

    switch (p_direction1) {
    case Direction::up:
        return p_direction2 == Direction::down;
    case Direction::left:
        return p_direction2 == Direction::right;
    case Direction::down:
        return p_direction2 == Direction::up;
    case Direction::right:
        return p_direction2 == Direction::left;
    default:
        return false;
    }
}

Direction Entity::getOppositeDirection() const {
    switch (m_currentDirection) {
    case Direction::up:
        return Direction::down;
    case Direction::left:
        return Direction::right;
    case Direction::down:
        return Direction::up;
    case Direction::right:
        return Direction::left;
    default:
        return Direction::none;
    }
}

Entity::Entity(Sprite p_sprite): GameObject(p_sprite) {
    
    m_size = Vector2D(60.0f, 60.0f);
    m_currentNode = getNodeByIndex(baseEntranceNodeIndex);
    m_previousNode = getNodeByIndex(0);
    m_position = m_currentNode->getPosition();
    m_velocity = Vector2D();
    m_speed = 0.0f;
    /*m_currentDirection = Direction::left;
    m_desiredDirection = Direction::left;*/
}

Entity::Entity(Vector2D p_position) : GameObject(p_position) {
    m_velocity = Vector2D();
    m_speed = 0.0f;  
}

void Entity::update(float p_deltaTime) {
    //GameObject::update(p_deltaTime);

    m_sprite.animate(m_speed * 0.01f, p_deltaTime);

    auto previousNode = getNodeByPosition(m_position);

    m_position += m_velocity * (m_speed * p_deltaTime);

    m_currentNode = getNodeByPosition();

    if (previousNode != m_currentNode) {
        m_currentNode = getNodeByPosition();
        m_previousNode = previousNode;
    }
    m_previousNode = m_currentNode;
}

void Entity::render() {
    GameObject::render();   
}

void Entity::renderWireframe() {
   GameObject::renderWireframe();   
}

void Entity::restart(int p_nodeIndex, Direction p_direction) {
    m_currentNode = getNodeByIndex(p_nodeIndex);
    m_position = m_currentNode->getPosition();  
    m_currentDirection = p_direction;
    m_desiredDirection = p_direction;
}

void Entity::setCurrentFramesRange(int p_startingFrame, int p_endingFrame) {
    m_sprite.setCurrentFramesRange(p_startingFrame, p_endingFrame);
}

void Entity::setAnimationDelay(float p_animationDelay) {
    m_sprite.setAnimationDelay(p_animationDelay);
}

float Entity::getCurrentFrame() {
    return m_sprite.getCurrentFrameIndex();
}

Direction Entity::getCurrentDirection() const {
    return m_currentDirection;
}

void Entity::setPositionByNode(const int p_nodeIndex) {
    m_currentNode = getNodeByIndex(p_nodeIndex);
    m_position = m_currentNode->getPosition() - Vector2D(1.0f, 0.0f);   
}

void Entity::setCurrentDirection(const Direction p_direction) {
    if (m_currentDirection != p_direction) m_currentDirection = p_direction;
}

void Entity::setDesiredDirection(const Direction p_direction) {
    if (m_desiredDirection != p_direction) m_desiredDirection = p_direction;
}

void Entity::setSpeed(const float p_speed) {
    if (m_speed != p_speed) m_speed = p_speed;
}

void Entity::setDefaultPosition() {
}
 
GraphNode* Entity::getCurrentNode() const {
    return m_currentNode;
}

GraphNode* Entity::getNextNode() const {
    return m_nextNode;
}

GraphNode* Entity::getPreviousNode() const {
    return m_previousNode;
}