#ifndef _GAMEWORLD_H
#define _GAMEWORLD_H

#include "Player.h"
#include "Enemy.h"
#include "Dot.h"

class Graph;
class InputManager;

class GameWorld {

private:

	GameObject* m_background;

	Player* m_player;

	Enemy* m_blinky;
	Enemy* m_pinky;
	Enemy* m_inky;
	Enemy* m_clyde;

	std::vector<Dot*> m_dots;

	Graph* m_graph;
	InputManager* m_inputManager;

	float m_deltaTime;
	float m_previousTime;

public:

	void init();

	void initDots();

	void update(float p_deltaTime);
	void render();
	void renderWireframe();

	virtual void keyboard(int, int, int);
	virtual void keyboardUp(int, int, int);
	virtual void mouse(int p_button, int p_state, int p_x, int p_y);
	virtual void reshape(int p_w, int p_h);
	virtual void display();
	virtual void idle();
};
#endif