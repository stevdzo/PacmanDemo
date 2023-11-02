#include "GameObject.h"

GameObject::GameObject() {
	m_position = Vector2D(0.0f, 0.0f);
	m_size = gv::defaultSize;
	m_wireframeColor = gv::gameObjectWireframeColor;
}

GameObject::GameObject(Vector2D p_position) {
	m_position = p_position;
	m_size = gv::defaultSize;
	m_wireframeColor = gv::gameObjectWireframeColor;
}

void GameObject::update(float p_deltaTime) {

}

void GameObject::render() {

}

void GameObject::renderWireframe() {

	if (gv::toggleWireframe) {		
		glBegin(GL_LINE_LOOP);		
		glColor3fv(m_wireframeColor.toArray());
			glVertex2f(m_position.x - m_size.x / 2, m_position.y - m_size.y / 2);
			glVertex2f(m_position.x + m_size.x / 2, m_position.y - m_size.y / 2);
			glVertex2f(m_position.x + m_size.x / 2, m_position.y + m_size.y / 2);
			glVertex2f(m_position.x - m_size.x / 2, m_position.y + m_size.y / 2);
		glEnd();
	}
}

void GameObject::setPosition(Vector2D p_position) {
	m_position = p_position;
}

Vector2D GameObject::getPosition() const {
	return m_position;
}

void GameObject::setSize(Vector2D p_size) {
	m_size = p_size;
}

Vector2D GameObject::getSize() const {
	return m_size;
}
