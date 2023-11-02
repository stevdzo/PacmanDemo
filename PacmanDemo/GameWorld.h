#ifndef _GAMEWORLD_H
#define _GAMEWORLD_H

#include "Player.h"

class Graph;
class InputManager;

class GameWorld {

private:

	Player* m_player;

	Graph* m_graph;
	InputManager* m_inputManager;

	float m_deltaTime;
	float m_previousTime;

public:

	void init();

	void update(float p_deltaTime);
	void render();

	virtual void keyboard(int, int, int);
	virtual void keyboardUp(int, int, int);
	virtual void mouse(int p_button, int p_state, int p_x, int p_y);
	virtual void reshape(int p_w, int p_h);
	virtual void display();
	virtual void idle();
};
#endif