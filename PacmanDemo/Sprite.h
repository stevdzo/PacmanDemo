#ifndef _SPRITE_H
#define _SPRITE_H

#include "SOIL2.h"
#include "Globals.h"

class Sprite {

private:

	int m_texture;
	int m_currentFrame;
	int m_numberOfFrames;
	int m_numberOfFramesX;
	int m_numberOfFramesY;
	int m_startingFrame;
	int m_endingFrame;
	float m_animationDelay;
	float m_animationElapsedTime;
	bool m_isTransparent;
	bool m_hasAnimationChanged;
	bool m_isLooped;

public:

	Sprite();
	Sprite(const char*, const int = numberOfFramesX, const int = numberOfFramesY, const bool = true);

	void loadTexture(const char* p_fileName);

	void setCurrentFrame(int p_currentFrame);
	void setCurrentFramesRange(int p_start, int p_end);

	int getCurrentFrame();
	int getTexture();
	int getNumberOfFramesX();
	int getNumberOfFramesY();

	float getAnimationDelay();
	float getAnimationElapsedTime();

	void setAnimationDelay(float);
	void setAnimationElapsedTime(float);

	bool isTransparent();

	bool hasAnimationChanged();
	void hasAnimationChanged(bool);

	bool isLooped();
	void isLooped(bool);

	void animate(float p_speed, float p_deltaTime);
};
#endif