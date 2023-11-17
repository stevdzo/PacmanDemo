#include "GameObject.h"

GameObject::GameObject() {
	//m_sprite = nullptr;
	m_position = Vector2D(0.0f, 0.0f);
	m_size = defaultSize;
	m_wireframeColor = gameObjectWireframeColor;
}

GameObject::GameObject(Sprite p_sprite) : m_sprite(p_sprite) {
	m_position = Vector2D(0.0f, 0.0f);
	m_size = defaultSize;
	m_wireframeColor = gameObjectWireframeColor;
}

GameObject::GameObject(Vector2D p_position) : m_position(p_position) {
	m_size = defaultSize;
	m_wireframeColor = gameObjectWireframeColor;
}

void GameObject::update(float p_deltaTime) {
	m_sprite.animate(p_deltaTime);
}

void GameObject::render() {

	if (toggleRender) {

		glColor3fv(defaultWireframeColor.toArray());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_sprite.getCurrentFrame());

		GLfloat x = m_position.x;
		GLfloat y = m_position.y;

		GLfloat w = m_size.x;
		GLfloat h = m_size.y;

		GLfloat texWidth = (GLfloat)m_sprite.getTextureIndex() / (GLfloat)m_sprite.getNumberOfFramesX();
		GLfloat texHeight = (GLfloat)m_sprite.getTextureIndex() / (GLfloat)m_sprite.getNumberOfFramesY();

		GLfloat u = 0.0f;
		GLfloat v = 0.0f;

		if (m_sprite.getTextureIndex() < m_sprite.getNumberOfFramesX() * m_sprite.getNumberOfFramesY()) {

			GLuint m_currentY = m_sprite.getCurrentFrameIndex() / m_sprite.getNumberOfFramesX();
			GLuint m_currentX = m_sprite.getCurrentFrameIndex() - m_currentY * m_sprite.getNumberOfFramesX();

			u = (GLfloat)m_currentX * texWidth;
			v = (GLfloat)m_currentY * texHeight;
		}
		glBegin(GL_QUADS);
		glTexCoord2f(u, v + texHeight);				glVertex2f(m_position.x - m_size.x / 2, m_position.y - m_size.y / 2);
		glTexCoord2f(u + texWidth, v + texHeight);	glVertex2f(m_position.x + m_size.x / 2, m_position.y - m_size.y / 2);
		glTexCoord2f(u + texWidth, v);				glVertex2f(m_position.x + m_size.x / 2, m_position.y + m_size.y / 2);
		glTexCoord2f(u, v);							glVertex2f(m_position.x - m_size.x / 2, m_position.y + m_size.y / 2);
		glEnd();

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);

	}
}

void GameObject::renderWireframe() {

	if (toggleWireframe) {	
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
