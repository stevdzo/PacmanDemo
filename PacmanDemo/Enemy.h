#ifndef _ENEMY_H
#define _ENEMY_H

#include "Entity.h"
#include "Player.h"
#include "AStar.h"

enum class EnemyState {
	idle = 0,
	wander = 1,
	follow_path = 2
};

class Enemy : public Entity {

private:

	AStar m_astar;

	Player* m_player;

	int m_currentNodeIndex;

	bool m_pathChanged;

	EnemyState m_enemyState;

	std::vector<GraphNode*> m_path;

public:

	Enemy(Player* m_player);

	void update(float p_deltaTime) override;
	void render() override;
	void renderWireframe() override;
	void renderPath();

	void findShortestPath(GraphNode* p_targetNode);

	void moveEnemy();
	void wander();
	void followPath();

	bool onEnemyNodeChange();
	bool onPlayerNodeChange();
};
#endif