#include "Entity.h"

// Gets the node positioned next to current node by given direction
int Entity::getAdjacentNodeIndex(GraphNode* p_currentNode, Direction p_direction) const {   
    return Graph::getInstance()->getAdjacentNodeIndex(p_currentNode, p_direction);
}

// Gets the node from graph by index
GraphNode* Entity::getNodeByIndex(int p_index) const {
    return Graph::getInstance()->getMatrixAsVector(Graph::getInstance()->getNodes())[p_index];
}

// Gets the node from graph by current position of entity
GraphNode* Entity::getNodeByPosition() const {
    return Graph::getInstance()->getNodeByPosition(m_position);
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
    if (m_currentNode->getIndex() == gv::leftPortalIndex && m_currentDirection == Direction::left)
        m_position = getNodeByIndex(gv::rightPortalIndex)->getPosition();
    if (m_currentNode->getIndex() == gv::rightPortalIndex && m_currentDirection == Direction::right)
        m_position = getNodeByIndex(gv::leftPortalIndex)->getPosition();
}

Entity::Entity(): GameObject() {

    m_currentNode = getNodeByIndex(453);
    m_position = m_currentNode->getPosition();
    m_velocity = Vector2D();
    m_speed = 0.0f;
    m_isMoving = false;
    m_currentDirection = Direction::left;
    m_desiredDirection = Direction::left;
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
    GameObject::update(p_deltaTime);

    m_position += m_velocity * (m_speed * p_deltaTime);




    setVelocityByDirection();
    checkForPortal();

    m_currentNode = getNodeByPosition();
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

GLboolean Entity::onEntityMoveRight() {
    return GLboolean();
}

GLboolean Entity::onEntityMoveLeft() {
    return GLboolean();
}

GLboolean Entity::onEntityMoveUp() {
    return GLboolean();
}

GLboolean Entity::onEntityMoveDown() {
    return GLboolean();
}

GLboolean Entity::onEntityMoveStop() {
    return GLboolean();
}

GLboolean Entity::onEntityMovement(GLint) {
    return GLboolean();
}