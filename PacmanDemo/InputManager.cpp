#include "InputManager.h"

InputManager* InputManager::instance = nullptr;

void InputManager::onKeyboardKeyClicked()
{
}

InputManager::InputManager(Player* p_player) {
    m_player = p_player;
    m_isKeyDown = false;
}

InputManager* InputManager::getInstance(Player* p_player) {
    if (instance == nullptr) {
        instance = new InputManager(p_player);
    }
    return instance;
}

void InputManager::keyboardSpec(int p_key, int p_x, int p_y) {

    if (!m_isKeyDown) {
        m_isKeyDown = true;

        std::cout << "MOVING" << std::endl;

        m_player->onPlayerMovement(p_key);
    }
}

void InputManager::keyboardSpecUp(int p_key, int p_x, int p_y) {
    
}

void InputManager::keyboard(int p_key, int p_x, int p_y) {

    m_keyDown[p_key] = true;
    m_player->onPlayerMovement(p_key);
}

void InputManager::keyboardUp(int p_key, int p_x, int p_y) {

    m_keyDown[p_key] = false;

    switch (p_key) {
    case '1': {
        toggleWireframe = !toggleWireframe;
        toggleRender = !toggleRender;
    }
            break;
    }
}

void InputManager::joystick(unsigned int p_buttons, int p_x, int p_y, int p_z) {
    m_player->onPlayerJoystickMovement(p_x, p_y, p_z);
}

void InputManager::mouse(int p_button, int p_state, int p_x, int p_y) {
    if (p_button == GLUT_LEFT_BUTTON && p_state == GLUT_DOWN) {
       auto node = Graph::getInstance()->getNodeByPosition(Vector2D(p_x, screenHeight - p_y));
       if (node) {
           std::cout << "Index: " << node->getIndex() << std::endl;
           std::cout << "Type: " << static_cast<int>(node->getNodeType()) << std::endl;    
           /*for (auto& n : node->getConnectedNodes()) {
               std::cout << "Index of neighboor: " << n->getIndex() << std::endl;
           }*/
       }
    }
}

InputManager::~InputManager() {
    std::cout << "Deleting InputManager." << std::endl;
}