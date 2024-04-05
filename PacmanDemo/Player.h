#ifndef _PLAYER_H
#define _PLAYER_H

#include "Entity.h"
#include "Dot.h"

class Enemy;

class Player : public Entity {

private:

	int m_score;
	int m_health;
	bool m_isAlive;
	bool m_allDotsEaten;
	Vector<Enemy*> m_ghosts;
	Vector<GameObject*> m_pacLives;

public:

	Player(Sprite p_sprite);
	
	void update(float p_deltaTime) override;
	void render() override;
	void renderWireframe() override;

	void restart() override;
	void restartGame();

	int getScore(void) const;
	int getHealth(void) const;

	void setGhosts(const Vector<Enemy*>& p_ghosts);

	void createUIHealth();

	void onDotCollision(Vector<Dot*>&);

	void checkGameOver();
	void checkGameWon();

	void onGhostCollision(Enemy* p_ghost);
	void onBigDotEaten();
	void onLifeLost();	
	void onGhostEaten(Enemy* p_ghost);	
	void onPlayerMovement(int = 0);
	void onPlayerJoystickMovement(int p_x, int p_y, int p_z);

	void isAlive(bool p_isAlive);
	bool isAlive(void);

	void isVisible(bool p_isVisible);
	bool isVisible(void);

	bool gameOver();

	void decreaseHealth();
	void increaseHealth();
	void checkForPortal();
	void manageSpeed();
	void resetAnimation();
	void setDefaultPosition();
	void setVelocityByDirection() override;
	void updateDirection() override;
};
#endif