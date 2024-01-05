#include "Drop.h"

Drop::Drop(Sprite p_sprite) : Entity(p_sprite) {

	m_value = 0;
}

void Drop::update(float p_deltaTime) {
	Entity::update(p_deltaTime);
}

void Drop::render() {
	Entity::render();
}

void Drop::renderWireframe() {
	Entity::renderWireframe();
}

Drop::~Drop() {
	std::cout << "Deleting Drop." << std::endl;
}