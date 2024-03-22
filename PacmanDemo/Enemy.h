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

class Enemy : public Entity {

private:

	AStar m_astar;

	Player* m_player;
	Enemy* m_blinky; // only for inky

	Vector2D m_offsetPosition;

	GraphNode* m_playerNode;
	GraphNode* m_scatterNode;
	GraphNode* m_eatenNode;	
	GraphNode* m_baseNode;	
	GraphNode* m_currentTargetNode;
	GraphNode* m_initialNode;

	GraphNode* m_inkyCurrentTargetNode; // only for inky

	int m_scatterNodeIndices[3];
	int m_baseNodeIndices[3];
	int m_currentNodeIndex;

	int m_animRange[8];

	bool m_frightenedDirectionChosen;
	bool m_frightened;
	bool m_insideBase;
	bool m_inBase;
	bool m_canGoOutsideBase;
	bool m_isEaten;
	bool m_hasGlobalStateChanged;

	bool m_canExitFrightened;

	bool m_isClydeInRange; // clyde
	bool m_clydeSwitchState; // clyde
	bool m_inkySwitchState; // inky

	EnemyState m_nextClydeState; // clyde
	EnemyState m_nextInkyState; // clyde

	EnemyState m_initialEnemyState;
	EnemyState m_currentEnemyState;
	EnemyState m_previousEnemyState;

	GhostType m_ghostType;

	std::vector<GraphNode*> m_path;	

public:

	Enemy(GhostType p_ghostType, Sprite p_sprite, const int* p_scatterNodeIndices, const int* p_baseNodeIndices, const int* p_animRange, const EnemyState p_initialState, Player* m_player);

	void update(float p_deltaTime) override;
	void render() override;
	void renderWireframe() override;
	void renderPath();

	void restart(int p_nodeIndex, Direction p_direction) override;

	void setVelocityByDirection() override;

	void onEnemyMoveUp();
	void onEnemyMoveDown();
	void onEnemyMoveLeft();
	void onEnemyMoveRight();

	void updateDirection() override;

	void setPositionByNode(const int p_index) override;

	void findShortestPath(GraphNode* p_targetNode);

	void moveEnemy();
	void followPath();
	void updateChaseTarget();

	void onChase();
	void onScatter();
	void onEaten();
	void onFrightened();
	void onBase();

	bool onEnemyNodeChange();
	bool onPlayerNodeChange();

	bool pathCompleted();

	bool isFrightened(void);
	void isFrightened(bool);

	bool isEaten(void);
	void isEaten(bool);

	bool isInsideBase(void);
	void isInsideBase(bool);

	GhostType getGhostType() const;
	GraphNode* getCurrentTargetNode() const;

	void setScatterNode(GraphNode* p_node);

	void toggleScatterNode();
	void toggleBaseNode();

	void flashOnFrightened(float p_deltaTime);

	void shouldExitBase(const bool p_insideBase);
	void shouldClydeSwitchState(float p_deltaTime);
	void shouldInkySwitchState(float p_deltaTime);

	void changeEnemyState(EnemyState p_enemyState);
	void returnPreviousEnemyState();
	void reverseDirection();

	void exitBase();

	void checkForPortal();

	void assignBlinkyToInky(Enemy* m_enemy);

	bool closeToNode();
	bool canCalculateNewDirection() const;

	bool canExitFrightened(void) const;
	void canExitFrightened(bool);

	bool allignedWithNode();

	EnemyState getCurrentState() const;

	std::vector<Direction> chooseDirectionWhenFrightened();
};
#endif