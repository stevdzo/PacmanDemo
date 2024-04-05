#include "Sprite.h"

Sprite::Sprite() {}

Sprite::Sprite(
	const char* p_fileName,
	const int p_numberOfFramesX,
	const int p_numberOfFramesY,
	bool p_isTransparent)
							 : m_numberOfFramesX(p_numberOfFramesX),
							   m_numberOfFramesY(p_numberOfFramesY),
							   m_isTransparent(p_isTransparent)
{
	m_currentFrame = 0;
	m_numberOfFrames = m_numberOfFramesX * m_numberOfFramesY;

	m_startingFrame = 0;
	m_endingFrame = 0;

	m_hasAnimationChanged = false;
	m_isLooped = true;

	m_animationDelay = normalAnimationDelay;
	m_animationElapsedTime = 0.0f;

	loadTexture(p_fileName);
}

void Sprite::loadTexture(const char* p_fileName) {
	m_texture = SOIL_load_OGL_texture(p_fileName, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
}

void Sprite::setCurrentFrame(int p_currentFrame) {
	this->m_currentFrame = p_currentFrame;
}

void Sprite::setCurrentFramesRange(int p_start, int p_end) {
	if (this->m_startingFrame != p_start &&
		this->m_endingFrame != p_end) {
		this->m_startingFrame = p_start;
		this->m_endingFrame = p_end;
		this->m_hasAnimationChanged = true;		
	}
}

int Sprite::getCurrentFrame() {
	return this->m_currentFrame;
}

int Sprite::getTexture() {
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

void Sprite::setAnimationDelay(float p_animationDelay) {
	m_animationDelay = p_animationDelay;
}

void Sprite::setAnimationElapsedTime(float p_animationElapsedTime) {
	m_animationElapsedTime = p_animationElapsedTime;
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

bool Sprite::isLooped() {
	return m_isLooped;
}

void Sprite::isLooped(bool p_isLooped) {
	m_isLooped = p_isLooped;
}

void Sprite::animate(float p_speed, float p_deltaTime) {

	m_animationElapsedTime += p_deltaTime * p_speed; 
	if (m_animationElapsedTime >= m_animationDelay) { // promena sličice nakon što protekne definisano vreme
		m_currentFrame++;
		if (m_currentFrame > m_endingFrame) // kada se dođe do poslednjeg frejma u intervalu, gleda se da li se animacija ponavlja
			if (m_isLooped)
				m_currentFrame = m_startingFrame; // ako se ponavlja, trenutni frejm se vraća na početak
			else
				m_currentFrame--; // ako se ne ponavlja vraća se na poslednji frejm

		m_animationElapsedTime = 0.0f; // resetuje se tajmer
	}
	if (m_hasAnimationChanged) { // ukoliko je animacija promenjena, resetuje se na početak novog frejma
		m_currentFrame = m_startingFrame;
		m_hasAnimationChanged = false;
	}
}