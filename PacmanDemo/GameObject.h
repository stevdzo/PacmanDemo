#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include "Globals.h"
#include "Vector2D.h"
#include "Vector3D.h"

class GameObject {

protected:

	Vector2D m_position;
	Vector2D m_size;

	Vector3D m_wireframeColor;

public:

	GameObject();
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