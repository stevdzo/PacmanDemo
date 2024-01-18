#ifndef _GAMEWORLD_H
#define _GAMEWORLD_H

#include "Player.h"
#include "Enemy.h"
#include "Dot.h"
#include "Drop.h"

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

	Drop* m_cherry;

	std::vector<Dot*> m_dots;
	std::vector<Enemy*> m_ghosts;	

	Graph* m_graph;
	InputManager* m_inputManager;
	AudioManager* m_audioManager;

	GameState m_gameState;

	float m_deltaTime;
	float m_previousTime;

public:

	void init();
	void initDots();

	void update(float p_deltaTime);
	void render();
	void renderWireframe();
	void renderUi();

	void onPausedGameState();
	void onRunningGameState();
	void onGameOverGameState();
	void onLifeLostGameState();

	void gameOver();
	void restart();

	virtual void keyboardSpec(int, int, int);
	virtual void keyboardSpecUp(int, int, int);
	virtual void keyboard(int, int, int);
	virtual void keyboardUp(int, int, int);
	virtual void mouse(int p_button, int p_state, int p_x, int p_y);
	virtual void reshape(int p_w, int p_h);
	virtual void display();
	virtual void idle();
};
#endif