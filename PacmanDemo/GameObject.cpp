#include "GameObject.h"

GameObject::GameObject() {
	//m_sprite = nullptr;
	m_position = Vector2D(0.0f, 0.0f);
	m_previousPosition = Vector2D(0.0f, 0.0f);
	m_size = Vector2D(32.0f, 32.0f);
	m_wireframeSize = Vector2D(wireframeSizeX, wireframeSizeY);
	m_wireframeColor = Vector3D(0.0f, 0.0f, 0.0f);
}

GameObject::GameObject(Sprite p_sprite) : m_sprite(p_sprite), m_isVisible(true) {
	m_position = Vector2D(0.0f, 0.0f);
	m_previousPosition = Vector2D(0.0f, 0.0f);
	m_size = Vector2D(32.0f, 32.0f);
	m_wireframeSize = Vector2D(wireframeSizeX, wireframeSizeY);
	m_wireframeColor = Vector3D(0.0f, 0.0f, 0.0f);
}

GameObject::GameObject(Vector2D p_position) : m_position(p_position), m_previousPosition(p_position) {
	m_size = Vector2D(32.0f, 32.0f);
	m_wireframeSize = Vector2D(wireframeSizeX, wireframeSizeY);
	m_wireframeColor = Vector3D(0.0f, 0.0f, 0.0f);
}

void GameObject::update(float p_deltaTime) {
	m_sprite.animate(p_deltaTime, 1.0f);
}

void GameObject::render() {

	if (toggleRender && m_isVisible) {

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_sprite.getCurrentFrame());

		float x = m_position.x;
		float y = m_position.y;

		float w = m_size.x;
		float h = m_size.y;

		float texWidth = (float)m_sprite.getTextureIndex() / (float)m_sprite.getNumberOfFramesX();
		float texHeight = (float)m_sprite.getTextureIndex() / (float)m_sprite.getNumberOfFramesY();

		float u = 0.0f;
		float v = 0.0f;

		if (m_sprite.getTextureIndex() < m_sprite.getNumberOfFramesX() * m_sprite.getNumberOfFramesY()) {

			unsigned int m_currentY = m_sprite.getCurrentFrameIndex() / m_sprite.getNumberOfFramesX();
			unsigned int m_currentX = m_sprite.getCurrentFrameIndex() - m_currentY * m_sprite.getNumberOfFramesX();

			u = (float)m_currentX * texWidth;
			v = (float)m_currentY * texHeight;
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

	if (toggleWireframe)
		drawRectangle(m_position.x, m_position.y, m_wireframeSize.x, m_wireframeSize.y, 1.0f, 1.0f, 1.0f, GL_LINE_LOOP);		
}

void GameObject::isVisible(bool p_isVisible) {
	m_isVisible = p_isVisible;
}

bool GameObject::isVisible(void) const {
	return m_isVisible;
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

void GameObject::setWireFrameColor(Vector3D p_wireframeColor) {
	m_wireframeColor = p_wireframeColor;
}

Vector3D GameObject::getWireFrameColor() const {
	return m_wireframeColor;
}