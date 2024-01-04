#include "Dot.h"

Dot::Dot(Sprite p_sprite, DotType p_type) : GameObject(p_sprite), m_type(p_type) {

    switch (m_type) {  
    case DotType::small:
        m_size = Vector2D(8.0f, 8.0f);
        m_value = 10;
        break;
    case DotType::big:
        m_size = Vector2D(32.0f, 32.0f);
        m_value = 50;
        break;
    }
    m_wireframeColor = Vector3D(1.0f, 1.0f, 1.0f);
}

Dot::~Dot() {
}

void Dot::setValue(int p_value) {
    m_value = p_value;
}

int Dot::getValue(void) {
    return m_value;
}

DotType Dot::getType() const {
    return m_type;
}

void Dot::update(float p_deltaTime) {
    GameObject::update(p_deltaTime);
}

void Dot::render() {
    GameObject::render();
}

void Dot::renderWireframe() {

    GameObject::renderWireframe();
    if (toggleWireframe) {       
        glBegin(GL_POLYGON);
        glColor3fv(m_wireframeColor.toArray());
        glVertex2f(m_position.x - m_size.x / 2, m_position.y - m_size.y / 2);
        glVertex2f(m_position.x + m_size.x / 2, m_position.y - m_size.y / 2);
        glVertex2f(m_position.x + m_size.x / 2, m_position.y + m_size.y / 2);
        glVertex2f(m_position.x - m_size.x / 2, m_position.y + m_size.y / 2);
        glEnd();
    }
}