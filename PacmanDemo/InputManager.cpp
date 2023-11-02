#include "InputManager.h"

InputManager* InputManager::instance = nullptr;

void InputManager::onKeyboardKeyClicked()
{
}

InputManager::InputManager(Player* p_player) {
    m_player = p_player;
}

InputManager* InputManager::getInstance(Player* p_player) {
    if (instance == nullptr) {
        instance = new InputManager(p_player);
    }
    return instance;
}

void InputManager::keyboard(int p_key, int p_x, int p_y) {
    m_player->onPlayerMovement(p_key);
}

void InputManager::keyboardUp(int p_key, int p_x, int p_y) {

    if (p_key == '1') {
        gv::toggleWireframe = !gv::toggleWireframe;
    }
}

void InputManager::mouse(int p_button, int p_state, int p_x, int p_y) {

}

InputManager::~InputManager() {
    std::cout << "Deleting InputManager." << std::endl;
}