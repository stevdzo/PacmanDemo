#ifndef _SPRITE_H
#define _SPRITE_H

#include "SOIL.h"
#include "Globals.h"

class Sprite {

private:

	int* m_textures;

	int m_texture;
	int m_textureIndex;
	int m_currentFrame;
	int m_numberOfFrames;
	int m_numberOfFramesX;
	int m_numberOfFramesY;
	int m_startingFrame;
	int m_endingFrame;

	int m_animSpriteRange[1][4];

	float m_animationDelay;
	float m_animationElapsedTime;

	bool m_isTextureLoaded;
	bool m_isTransparent;
	bool m_isSpriteSheet;
	bool m_hasAnimationChanged;

public:

	Sprite();
	Sprite(const char*, int, int, bool = true);

	void setCurrentFrame(int p_currentFrame);
	void setCurrentFramesRange(int p_start, int p_end);

	int getTextureIndex();
	int getCurrentFrameIndex();
	int getCurrentFrame();
	int getNumberOfFramesX();
	int getNumberOfFramesY();

	float getAnimationDelay();
	float getAnimationElapsedTime();

	void setAnimationDelay(float);
	void setAnimationElapsedTime(float);

	bool isTransparent();
	bool hasAnimationChanged();
	void hasAnimationChanged(bool);

	void animate(float p_deltaTime, float p_speed);
};
#endif