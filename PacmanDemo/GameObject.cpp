#include "GameObject.h"

GameObject::GameObject() {
	m_position = Vector2D(0.0f, 0.0f);
	m_size = Vector2D(50.0f, 50.0f);
	m_wireframeColor = Vector3D(0.0f, 1.0f, 0.0f);
}

GameObject::GameObject(Vector2D p_position) {
	m_position = p_position;
	m_size = Vector2D(50.0f, 50.0f);
	m_wireframeColor = Vector3D(0.0f, 1.0f, 0.0f);
}

void GameObject::update(float p_deltaTime) {

}

void GameObject::render() {

}

void GameObject::renderWireframe() {

	glBegin(GL_LINE_LOOP);
	glColor3fv(m_wireframeColor.toArray());
		glVertex2f(m_position.x - m_size.x / 2, m_position.y - m_size.y / 2);
		glVertex2f(m_position.x + m_size.x / 2, m_position.y - m_size.y / 2);
		glVertex2f(m_position.x + m_size.x / 2, m_position.y + m_size.y / 2);
		glVertex2f(m_position.x - m_size.x / 2, m_position.y + m_size.y / 2);
	glEnd();
}