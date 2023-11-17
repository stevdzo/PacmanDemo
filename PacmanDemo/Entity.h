#ifndef _ENTITY_H
#define _ENTITY_H

#include "Graph.h"

class Entity : public GameObject {

protected:

	GraphNode* m_currentNode;
	GraphNode* m_nextNode;
	GraphNode* m_previousNode;

	Vector2D m_velocity;

	float m_speed;

	bool m_isMoving;

	Direction m_currentDirection;
	Direction m_desiredDirection;

	virtual int getNodeIndexByDirection(Direction p_direction) const;
	virtual GraphNode* getNodeByIndex(int p_index) const;
	virtual GraphNode* getNodeByDirectionFromCurrentNode(Direction p_direction) const;
	virtual GraphNode* getNodeByDirectionFromCustomNode(GraphNode* p_node, Direction p_direction) const;
	virtual GraphNode* getNodeByPosition() const;
	virtual GraphNode* getNodeByPosition(Vector2D p_position) const;
	virtual Direction getDirectionByNextNode() const;
	virtual Direction getDirectionByGivenNode(GraphNode* p_node) const;
	virtual void setVelocityByDirection();
	virtual void checkForPortal();
	virtual void updateDirection();
	virtual bool isValidDirection() const;
	virtual bool isOppositeDirection(Direction p_direction1, Direction p_direction2) const;

public:

	Entity(Sprite p_sprite);
	Entity(Vector2D p_position);

	virtual void update(float p_deltaTime);
	virtual void render();
	virtual void renderWireframe();

	GraphNode* getCurrentNode() const;
	GraphNode* getNextNode() const;
	GraphNode* getPreviousNode() const;
};
#endif