#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include "Sprite.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "AudioManager.h"
#include "Globals.h"

class GameObject {

protected:

	Sprite m_sprite;

	std::vector<Sprite> m_mazeFlashBgrs; // bgr only

	Vector2D m_position;
	Vector2D m_previousPosition;
	Vector2D m_size;
	Vector2D m_wireframeSize;

	Vector3D m_wireframeColor;

	bool m_isVisible;

public:

	GameObject();
	GameObject(Sprite p_sprite);
	GameObject(Vector2D p_position);

	virtual void update(float p_deltaTime);
	virtual void render();
	virtual void renderWireframe();

	void isVisible(bool);
	bool isVisible(void) const;

	void addBgrSprites(Sprite p_spr);

	void setSprite(Sprite p_spr);
	Sprite getSprite() const;

	void setPosition(Vector2D p_position);
	Vector2D getPosition() const;

	void setSize(Vector2D p_size);
	Vector2D getSize() const;

	void setWireFrameColor(Vector3D p_wireframeColor);
	Vector3D getWireFrameColor() const;

	// BGR ONLY
	void flashBackground(float p_deltaTime);
	void resetSprite();
};
#endif