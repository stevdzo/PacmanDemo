#ifndef _PLAYER_H
#define _PLAYER_H

#include "Entity.h"
#include "Dot.h"

class Player : public Entity {

private:

	int m_score;
	int m_health;

public:

	Player(Sprite p_sprite);
	
	void update(float p_deltaTime) override;
	void render() override;
	void renderWireframe() override;

	int getScore(void) const;
	int getHealth(void) const;

	void eatDot(std::vector<Dot*>&);

	void onPlayerMovement(int = 0);

	void setVelocityByDirection() override;
	void updateDirection() override;
};
#endif