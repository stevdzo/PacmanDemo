#ifndef _ENTITY_H
#define _ENTITY_H

#include "Graph.h"

class Entity : public GameObject {

protected:

	GraphNode* m_currentNode;
	GraphNode* m_nextNode;
	GraphNode* m_previousNode;
	Vector2D m_velocity;	
	Direction m_currentDirection;
	Direction m_desiredDirection;
	Direction m_previousDirection;
	float m_speed;

	virtual int getDistanceInNodes(GraphNode* m_targetNode) const;
	virtual int getNodeIndexByDirection(Direction p_direction) const;
	virtual GraphNode* getNodeByIndex(int p_index) const;
	virtual GraphNode* getNodeByDirectionFromCurrentNode(Direction p_direction) const;
	virtual GraphNode* getNodeInDirection(GraphNode* p_node, Direction p_direction, const int p_tileDistance) const;
	virtual GraphNode* getNodeByPosition() const;
	virtual GraphNode* getNodeByPosition(Vector2D p_position) const;	
	virtual GraphNode* getNodeByTwoTargetsDoubled(GraphNode* p_node1, GraphNode* p_node2, Direction p_direction) const;
	virtual Direction getDirectionByNextNode() const;
	virtual Direction getDirectionByGivenNode(GraphNode* p_node) const;

	virtual void setVelocityByDirection();
	virtual void checkForPortal();
	virtual void updateDirection();
	virtual bool isValidDirection(const Direction p_direction) const;
	virtual bool isOppositeDirection(Direction p_direction1, Direction p_direction2) const;	

public:

	Entity(Sprite p_sprite);
	Entity(Vector2D p_position);

	virtual void update(float p_deltaTime);
	virtual void render();
	virtual void renderWireframe();

	virtual void restart();

	virtual bool setCurrentFramesRange(int p_startingFrame, int p_endingFrame, bool p_isLooped = true);
	virtual void setAnimationDelay(float p_animationDelay);

	virtual float getTexture();

	Direction getCurrentDirection() const;
	Direction getOppositeDirection() const;

	virtual void setPositionByNode(const int p_nodeIndex);
	void setCurrentDirection(const Direction p_direction);

	void setDesiredDirection(const Direction p_direction);

	void setSpeed(const float p_speed);

	float distanceTo(const Vector2D p_position);
	float distanceToSq(const Vector2D p_position);

	GraphNode* getCurrentNode() const;
	GraphNode* getNextNode() const;
	GraphNode* getPreviousNode() const;
};
#endif