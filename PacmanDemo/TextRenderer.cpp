#include "TextRenderer.h"

TextRenderer* TextRenderer::instance = nullptr;

TextRenderer::TextRenderer() {
}

void TextRenderer::drawStrokeText(char* p_string, int p_x, int p_y, const float p_r, const float p_g, const float p_b, const float p_scale) {
    char* c;

    glColor3f(p_r, p_g, p_b);
    glPushMatrix();
    glTranslatef(p_x, p_y + 8, 0.0);
    glLineWidth(2.0);
    glScalef(0.14f*p_scale, 0.12f* p_scale, 0.0);

    for (c = p_string; *c != '\0'; c++) 
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
    
    glPopMatrix();

    glColor3f(1.0f, 1.0f, 1.0f);
}

TextRenderer* TextRenderer::getInstance() {
    if (instance == nullptr) {
        instance = new TextRenderer();
    }
    return instance;
}

TextRenderer::~TextRenderer() {
}