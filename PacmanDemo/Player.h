#ifndef _PLAYER_H
#define _PLAYER_H

#include "Entity.h"
#include "Dot.h"

class Enemy;

class Player : public Entity {

private:

	int m_score;
	int m_health;

	std::vector<GameObject*> m_pacLives;

public:

	Player(Sprite p_sprite);
	
	void update(float p_deltaTime) override;
	void render() override;
	void renderWireframe() override;

	void restart(int p_nodeIndex, Direction p_direction) override;

	int getScore(void) const;
	int getHealth(void) const;

	void eatDot(std::vector<Dot*>&, std::vector<Enemy*>& p_ghosts);

	void onGhostCollision(Enemy* p_ghost);

	void onPlayerMovement(int = 0);

	void setDefaultPosition() override;

	void setVelocityByDirection() override;
	void updateDirection() override;
};
#endif