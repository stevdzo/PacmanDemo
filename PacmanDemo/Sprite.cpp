#include "Sprite.h"

Sprite::Sprite() {}

Sprite::Sprite(
	const char* p_fileName,
	int p_numberOfFramesX,
	int p_numberOfFramesY,
	bool p_isTransparent)
	: m_numberOfFramesX(p_numberOfFramesX),
	m_numberOfFramesY(p_numberOfFramesY)
{

	m_textureIndex = 0;
	m_currentFrame = 0;
	m_numberOfFrames = p_numberOfFramesX * p_numberOfFramesY;

	m_startingFrame = 2;
	m_endingFrame = 2; // for pacman sprite

	m_hasAnimationChanged = false;

	m_animationDelay = 0.1f;
	m_animationElapsedTime = 0.0f;

	std::cout << p_numberOfFramesX << std::endl;
	std::cout << p_numberOfFramesY << std::endl;

	int texture = SOIL_load_OGL_texture(p_fileName, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);

	if (!texture) {
		const char* errorMessage = SOIL_last_result();
		std::cout << "Texture loading failed: " << errorMessage << "\n" << std::endl;
	}
	else {
		this->m_texture = texture;
		this->m_textureIndex++;

		if (this->m_textureIndex == 1 && this->m_numberOfFramesX * this->m_numberOfFramesY > 1) {
			this->m_isSpriteSheet = true;
		}
		else {
			this->m_isSpriteSheet = false;
		}
		this->m_isTransparent = p_isTransparent;
	}
}

void Sprite::setCurrentFrame(int p_currentFrame) {
	this->m_currentFrame = p_currentFrame;
}

void Sprite::setCurrentFramesRange(int p_start, int p_end) {
	this->m_startingFrame = p_start;
	this->m_endingFrame = p_end;
	this->m_hasAnimationChanged = true;
}

int Sprite::getTextureIndex() {

	return this->m_textureIndex;
}

int Sprite::getCurrentFrameIndex() {

	return this->m_currentFrame;
}

int Sprite::getCurrentFrame() {

	return this->m_texture;
}

int Sprite::getNumberOfFramesX() {

	return this->m_numberOfFramesX;
}

int Sprite::getNumberOfFramesY() {

	return this->m_numberOfFramesY;
}

float Sprite::getAnimationDelay() {
	return this->m_animationDelay;
}

float Sprite::getAnimationElapsedTime() {

	return this->m_animationElapsedTime;
}

bool Sprite::isTransparent() {

	return this->m_isTransparent;
}

bool Sprite::hasAnimationChanged() {
	return this->m_hasAnimationChanged;
}

void Sprite::hasAnimationChanged(bool p_hasAnimationChanged) {
	this->m_hasAnimationChanged = p_hasAnimationChanged;
}

void Sprite::animate(float p_deltaTime) {

	m_animationElapsedTime += p_deltaTime;
	if (m_animationElapsedTime >= m_animationDelay) {
		m_currentFrame++;
		if (m_currentFrame > m_endingFrame) m_currentFrame = m_startingFrame;
		m_animationElapsedTime = 0.0f;
	}
	if (m_hasAnimationChanged) {
		m_currentFrame = m_startingFrame;
		m_hasAnimationChanged = false;
	}
}