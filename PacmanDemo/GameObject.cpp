#include "GameObject.h"

GameObject::GameObject() {
	m_position = Vector2D(0.0f, 0.0f);
	m_previousPosition = Vector2D(0.0f, 0.0f);
	m_size = Vector2D(32.0f, 32.0f);
	m_wireframeSize = Vector2D(nodeSize, nodeSize);
	m_wireframeColor = Vector3D(0.0f, 0.0f, 0.0f);
}

GameObject::GameObject(Sprite p_sprite) : m_sprite(p_sprite), m_isVisible(true) {
	m_position = Vector2D(0.0f, 0.0f);
	m_previousPosition = Vector2D(0.0f, 0.0f);
	m_size = Vector2D(32.0f, 32.0f);
	m_wireframeSize = Vector2D(nodeSize, nodeSize);
	m_wireframeColor = Vector3D(0.0f, 0.0f, 0.0f);

	if (!p_sprite.isTransparent())
		m_mazeFlashBgrs.push_back(p_sprite);
}

GameObject::GameObject(Vector2D p_position) : m_position(p_position), m_previousPosition(p_position) {
	m_size = Vector2D(32.0f, 32.0f);
	m_wireframeSize = Vector2D(nodeSize, nodeSize);
	m_wireframeColor = Vector3D(0.0f, 0.0f, 0.0f);
}

void GameObject::update(float p_deltaTime) {
	m_sprite.animate(1.0, p_deltaTime);
}

void GameObject::render() {

	if (toggleRender && m_isVisible) {
		
		// transparentnost sprajta
		glEnable(GL_BLEND); 
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		// omogućavanje da se tekstura iscrta 
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_sprite.getTexture());

		float x = m_position.x;
		float y = m_position.y;

		float w = m_size.x;
		float h = m_size.y;

		float texWidth  = 1.0f / (float) m_sprite.getNumberOfFramesX();
		float texHeight = 1.0f / (float) m_sprite.getNumberOfFramesY();

		float u = 0.0f;
		float v = 0.0f;

		unsigned int currentY = m_sprite.getCurrentFrame() / m_sprite.getNumberOfFramesX();
		unsigned int currentX = m_sprite.getCurrentFrame() % m_sprite.getNumberOfFramesX();

		u = (float) currentX * texWidth;
		v = (float) currentY * texHeight;
	
		// texCoord zauzima opseg 0-1, tex tacke se lepe na tacke objekta
		glBegin(GL_QUADS);
		glTexCoord2f(u,            v + texHeight);	glVertex2f(x - w / 2, y - h / 2);
		glTexCoord2f(u + texWidth, v + texHeight);	glVertex2f(x + w / 2, y - h / 2);
		glTexCoord2f(u + texWidth, v);				glVertex2f(x + w / 2, y + h / 2);
		glTexCoord2f(u,            v);				glVertex2f(x - w / 2, y + h / 2);
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

void GameObject::addBgrSprites(Sprite p_spr) {
	m_mazeFlashBgrs.push_back(p_spr);
}

void GameObject::setSprite(Sprite p_sprite) {
	m_sprite = p_sprite;
}

Sprite GameObject::getSprite() const {
	return m_sprite;
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

void GameObject::flashBackground(float p_deltaTime) {
	((int) (nextLevelDelayTimer * 10) % 7 == 0) ?
		m_sprite = m_mazeFlashBgrs[2] :
		m_sprite = m_mazeFlashBgrs[1];
}

void GameObject::resetSprite() {
	m_sprite = m_mazeFlashBgrs[0];
}