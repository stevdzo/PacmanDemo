#include "Entity.h"

Entity::Entity(): GameObject() {

    m_currentNode = Graph::getInstance()->getMatrixAsVector(Graph::getInstance()->getNodes())[33];
    m_velocity = Vector2D();
    m_speed = 0.0f;
}

Entity::Entity(Vector2D p_position) : GameObject(p_position) {

    m_currentNode = Graph::getInstance()->getMatrixAsVector(Graph::getInstance()->getNodes())[33];
    m_velocity = Vector2D();
    m_speed = 0.0f;
}

void Entity::update(float p_deltaTime) {
    GameObject::update(p_deltaTime);
}

void Entity::render() {
    GameObject::render();   
}

void Entity::renderWireframe() {
    GameObject::renderWireframe();
    if (gv::toggleWireframe) {
        glBegin(GL_LINE_LOOP);
        glColor3fv(m_wireframeColor.toArray());
        glVertex2f(m_currentNode->getPosition().x - m_currentNode->getSize().x / 2, m_currentNode->getPosition().y - m_currentNode->getSize().y / 2);
        glVertex2f(m_currentNode->getPosition().x + m_currentNode->getSize().x / 2, m_currentNode->getPosition().y - m_currentNode->getSize().y / 2);
        glVertex2f(m_currentNode->getPosition().x + m_currentNode->getSize().x / 2, m_currentNode->getPosition().y + m_currentNode->getSize().y / 2);
        glVertex2f(m_currentNode->getPosition().x - m_currentNode->getSize().x / 2, m_currentNode->getPosition().y + m_currentNode->getSize().y / 2);
        glEnd();
    }
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
