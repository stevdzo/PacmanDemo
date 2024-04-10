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
	Enemy* m_blinky; // samo za Inky-a

	Vector2D m_offsetPosition;

	GraphNode* m_playerNode;
	GraphNode* m_scatterNode;
	GraphNode* m_eatenNode;	
	GraphNode* m_baseNode;	
	GraphNode* m_currentTargetNode;
	GraphNode* m_initialNode;

	int m_scatterNodeIndices[3];
	int m_baseNodeIndices[3];
	int m_animRange[8];

	bool m_frightenedDirectionChosen;
	bool m_frightened;
	bool m_canGoOutsideBase;

	EnemyState m_initialEnemyState;
	EnemyState m_currentEnemyState;
	EnemyState m_previousEnemyState;

	GhostType m_ghostType;

	Vector<GraphNode*> m_path;	

public:

	Enemy(GhostType p_ghostType, 
		Sprite p_sprite, 
		const int* p_scatterNodeIndices, 
		const int* p_baseNodeIndices,
		const int* p_animRange, 
		const EnemyState p_initialState, 
		Player* m_player);

	void update(float p_deltaTime) override;
	void render() override;
	void renderWireframe() override;
	void renderPath();
	void restart() override;

	void setVelocityByDirection() override;

	int onEnemyMoveUp();
	int onEnemyMoveDown();
	int onEnemyMoveLeft();
	int onEnemyMoveRight();

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

	bool setGhostFrightenedTextures();
	bool setGhostFrightenedFlashTextures();
	bool setGhostEatenTextures(const int index);
	bool setGhostDefaultTextures(const int index1, const int index2);

	GhostType getGhostType() const;
	GraphNode* getCurrentTargetNode() const;

	void setScatterNode(GraphNode* p_node);

	void toggleScatterNode();
	void toggleBaseNode();
	void flashOnFrightened(float p_deltaTime);
	void changeEnemyState(EnemyState p_enemyState);
	void returnPreviousEnemyState();
	void reverseDirection();
	void exitBase();
	void checkForPortal();
	void setSpeed(const float p_speed);
	void assignBlinkyToInky(Enemy* m_enemy); // inky

	void goToBase();
	void switchClydeToChase();
	void switchClydeToScatter();

	bool closeToNode();
	bool canUpdateChaseTarget();
	bool canCalculateNewDirection() const;	

	EnemyState getCurrentState() const;

	Vector<Direction> chooseDirectionWhenFrightened();
};
#endif