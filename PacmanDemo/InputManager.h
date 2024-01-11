#ifndef _INPUTMANAGER_H
#define _INPUTMANAGER_H

#include "Globals.h"
#include "Player.h"

class InputManager {

private:

	static InputManager* instance;

	Player* m_player;

	GLboolean m_keyDown[256];

	GLboolean m_isKeyPressed;

	void onKeyboardKeyClicked();

	InputManager(Player*);

public:

	static InputManager* getInstance(Player*);

	void keyboardSpec(int p_key, int p_x, int p_y);
	void keyboardSpecUp(int p_key, int p_x, int p_y);
	void keyboard(int p_key, int p_x, int p_y);
	void keyboardUp(int p_key, int p_x, int p_y);
	void mouse(int p_button, int p_state, int p_x, int p_y);

	~InputManager();
};
#endif