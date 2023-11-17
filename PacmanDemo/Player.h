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

	void eatDot(std::vector<Dot*>&);

	void onPlayerMovement(int = 0);
};
#endif