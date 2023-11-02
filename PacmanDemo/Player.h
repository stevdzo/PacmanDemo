#ifndef _PLAYER_H
#define _PLAYER_H

#include "Entity.h"

class Player : public Entity {

private:

	int m_score;

public:

	Player();
	
	void update(float p_deltaTime) override;
	void render() override;
	void renderWireframe() override;

	void onPlayerMoveRight();
	void onPlayerMoveLeft();
	void onPlayerMoveUp();
	void onPlayerMoveDown();
	void onPlayerMoveStop();
	void onPlayerMovement(int = 0);
};
#endif