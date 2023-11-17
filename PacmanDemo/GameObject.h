#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include "Sprite.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "Globals.h"

class GameObject {

protected:

	Sprite m_sprite;

	Vector2D m_position;
	Vector2D m_size;

	Vector3D m_wireframeColor;

public:

	GameObject();
	GameObject(Sprite p_sprite);
	GameObject(Vector2D p_position);

	virtual void update(float p_deltaTime);
	virtual void render();
	virtual void renderWireframe();

	void setPosition(Vector2D p_position);
	Vector2D getPosition() const;

	void setSize(Vector2D p_size);
	Vector2D getSize() const;
};
#endif