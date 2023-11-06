#ifndef _ENEMY_H
#define _ENEMY_H

#include "Entity.h"
#include "AStar.h"

class Enemy : public Entity {

private:

	AStar m_astar;

	std::vector<GraphNode*> m_path;

public:

	Enemy();

	void update(float p_deltaTime) override;
	void render() override;
	void renderWireframe() override;

	void findShortestPath(GraphNode* p_targetNode);
};
#endif