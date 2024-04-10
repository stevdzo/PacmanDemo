#ifndef _GAMEWORLD_H
#define _GAMEWORLD_H

#include "Player.h"
#include "Enemy.h"
#include "Dot.h"

class Graph;
class InputManager;
class AudioManager;
class TextRenderer;

class GameWorld {

private:

	GameObject* m_background;

	Player* m_player;

	Enemy* m_blinky;
	Enemy* m_pinky;
	Enemy* m_inky;
	Enemy* m_clyde;

	Vector<Dot*> m_dots;
	Vector<Enemy*> m_ghosts;

	GameState m_gameState;

	float m_deltaTime;
	float m_previousTime;

public:

	void init();
	void initDots();

	void update();
	void updateGhosts();
	void updatePlayer();

	void render();
	void renderWireframe();
	void renderUi();

	void onStartGameState();
	void onPausedGameState();
	void onRunningGameState();
	void onGameOverGameState();
	void onNextLevelGameState();
	void onLifeLostGameState();

	void manageFrightenedState();
	void manageRetreatingSound();
	void manageSirenSound();
	void manageCollision();
	void manageGhostStates();
	void manageBonusHealth();

	void adjustLevelStats();
	void restart();

	virtual void keyboardSpec(int, int, int);
	virtual void keyboardSpecUp(int, int, int);
	virtual void keyboard(int, int, int);
	virtual void keyboardUp(int, int, int);
	virtual void joystick(unsigned int, int, int, int);
	virtual void mouse(int p_button, int p_state, int p_x, int p_y);
	virtual void reshape(int p_w, int p_h);
	virtual void display();
	virtual void idle();
};
#endif