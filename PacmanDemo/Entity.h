#ifndef _ENTITY_H
#define _ENTITY_H

#include "Graph.h"

class Entity : public GameObject {

protected:

	GraphNode* m_currentNode;

	Vector2D m_velocity;

	float m_speed;

	bool m_isMoving;

	Direction m_currentDirection;
	Direction m_desiredDirection;

	virtual int getAdjacentNodeIndex(GraphNode* p_currentNode, Direction p_direction) const;
	virtual GraphNode* getNodeByIndex(int p_index) const;
	virtual GraphNode* getNodeByPosition() const;
	virtual void setVelocityByDirection();
	virtual void checkForPortal();

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