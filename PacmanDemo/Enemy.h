#ifndef _ENEMY_H
#define _ENEMY_H

#include "Entity.h"
#include "Player.h"
#include "AStar.h"

enum class GhostType {
	blinky = 0,
	pinky = 1,
	inky = 2,
	clyde = 3,
};

enum class EnemyState {
	none = 0,
	chase = 1,
	scatter = 2,
	eaten = 3,
	frightened = 4
};

class Enemy : public Entity {

private:

	AStar m_astar;

	Player* m_player;
	
	GraphNode* m_playerNode;
	GraphNode* m_scatterNode;
	GraphNode* m_eatenNode;	

	int m_scatterNodeIndices[3];
	int m_currentNodeIndex;

	bool m_frightenedDirectionChosen;

	EnemyState m_currentEnemyState;
	EnemyState m_previousEnemyState;

	GhostType m_ghostType;

	std::vector<GraphNode*> m_path;	

public:

	Enemy(GhostType p_ghostType, Sprite p_sprite, const int* p_scatterNodeIndices, Player* m_player);

	void update(float p_deltaTime) override;
	void render() override;
	void renderWireframe() override;
	void renderPath();

	void setVelocityByDirection() override;
	void updateDirection() override;

	void findShortestPath(GraphNode* p_targetNode);

	void moveEnemy();
	void followPath();
	void updateChaseTarget();

	void onChase();
	void onScatter();
	void onEaten();
	void onFrightened();

	bool onEnemyNodeChange();
	bool onPlayerNodeChange();

	bool pathCompleted();

	void toggleScatterNode();

	void changeEnemyState(EnemyState p_enemyState);
	void returnPreviousEnemyState();

	std::vector<Direction> chooseDirectionWhenFrightened();
};
#endif