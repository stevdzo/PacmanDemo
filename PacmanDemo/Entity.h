#ifndef _ENTITY_H
#define _ENTITY_H

#include "Graph.h"

class Entity : public GameObject {

protected:

	GraphNode* m_currentNode;

	Vector2D m_velocity;

	float m_speed;

public:

	Entity();
	Entity(Vector2D p_position);

	virtual void update(float p_deltaTime);
	virtual void render();
	virtual void renderWireframe();

	virtual GLboolean onEntityMoveRight();
	virtual GLboolean onEntityMoveLeft();
	virtual GLboolean onEntityMoveUp();
	virtual GLboolean onEntityMoveDown();
	virtual GLboolean onEntityMoveStop();
	virtual GLboolean onEntityMovement(GLint = 0);
};
#endif