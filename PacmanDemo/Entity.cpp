#include "Entity.h"

// Gets the node positioned next to current node by given direction
int Entity::getNodeIndexByDirection(Direction p_direction) const {
    return Graph::getInstance()->getNodeIndexByDirection(m_currentNode, p_direction);
}

// Gets the node from graph by index
GraphNode* Entity::getNodeByIndex(int p_index) const {
    return Graph::getInstance()->getMatrixAsVector(Graph::getInstance()->getNodes())[p_index];
}

// Gets the node from given direction by current node
GraphNode* Entity::getNodeByDirectionFromCurrentNode(Direction p_direction) const {
    return getNodeByIndex(getNodeIndexByDirection(p_direction));
}

// Gets the node from given direction by custom node
GraphNode* Entity::getNodeByDirectionFromCustomNode(GraphNode* p_node, Direction p_direction) const {
    return getNodeByIndex(Graph::getInstance()->getNodeIndexByDirection(p_node, p_direction));
}

// Gets the node from graph by current position of entity
GraphNode* Entity::getNodeByPosition() const {
    return Graph::getInstance()->getNodeByPosition(m_position);
}

// Gets the node by given position
GraphNode* Entity::getNodeByPosition(Vector2D p_position) const {
    return Graph::getInstance()->getNodeByPosition(p_position);
}

// Gets the direction between current and next node
Direction Entity::getDirectionByNextNode() const {
    return Graph::getInstance()->getDirectionByNode(m_currentNode, m_nextNode);
}

// Gets the direction between current and given ndoe
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

bool Entity::isValidDirection() const {
    return !(m_nextNode && (m_nextNode->isEmptyNode() || m_nextNode->isObstacle()));
    //return !(m_nextNode->isEmptyNode() || m_nextNode->isObstacle());
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

Entity::Entity(Sprite p_sprite): GameObject(p_sprite) {
    
    m_size = Vector2D(32.0f, 32.0f);
    m_currentNode = getNodeByIndex(453);
    m_currentNode = getNodeByIndex(441);
    m_position = m_currentNode->getPosition();
    m_velocity = Vector2D();
    m_speed = 0.0f;
    /*m_currentDirection = Direction::left;
    m_desiredDirection = Direction::left;*/
}

Entity::Entity(Vector2D p_position) : GameObject(p_position) {

    m_currentNode = getNodeByIndex(453);  
    m_position = m_currentNode->getPosition();
    m_velocity = Vector2D();
    m_speed = 0.0f;
    m_isMoving = false;
    m_currentDirection = Direction::left;
}

void Entity::update(float p_deltaTime) {
    //GameObject::update(p_deltaTime);
    m_sprite.animate(p_deltaTime, m_speed / 100.0f);

    Vector2D previousPosition = m_position;

    m_position += m_velocity * (m_speed * p_deltaTime);

    checkForPortal();

    m_currentNode = getNodeByPosition();
    m_previousNode = getNodeByPosition(previousPosition);
}

void Entity::render() {
    GameObject::render();   
}

void Entity::renderWireframe() {
    GameObject::renderWireframe();
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