#ifndef _INPUTMANAGER_H
#define _INPUTMANAGER_H

#include "Globals.h"
#include "Player.h"

class InputManager {

private:

	static InputManager* instance;
	Player* m_player;

	bool m_keyDown[256];

	InputManager();

public:

	static InputManager* getInstance();

	void keyboardSpec(int p_key, int p_x, int p_y);
	void keyboardSpecUp(int p_key, int p_x, int p_y);
	void keyboard(int p_key, int p_x, int p_y);
	void keyboardUp(int p_key, int p_x, int p_y);
	void joystick(unsigned int p_buttons, int p_x, int p_y, int p_z);
	void mouse(int p_button, int p_state, int p_x, int p_y);

	void setPlayer(Player*);

	~InputManager();
};
#endif