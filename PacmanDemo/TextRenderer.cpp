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

    for (c = p_string; *c != '\0'; c++) {

        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
    }
    glPopMatrix();

    glColor3f(1.0f, 1.0f, 1.0f);
}

TextRenderer::TextRenderer(const char* fontPath, int fontSize) {

    //if (FT_Init_FreeType(&ft) != 0) {
    //    // Handle error
    //}

    //if (FT_New_Face(ft, fontPath, 0, &face) != 0) {
    //    // Handle error
    //}

    //FT_Set_Pixel_Sizes(face, 0, fontSize);
}

TextRenderer* TextRenderer::getInstance(const char* p_fontPath, int p_fontSize) {
    if (instance == nullptr) {
        instance = new TextRenderer(p_fontPath, p_fontSize);
    }
    return instance;
}

TextRenderer* TextRenderer::getInstance() {
    if (instance == nullptr) {
        instance = new TextRenderer();
    }
    return instance;
}

TextRenderer::~TextRenderer() {
    /*FT_Done_Face(face);
    FT_Done_FreeType(ft);*/
}

void TextRenderer::renderText(const char* text, float x, float y) {

    //glColor3f(1.0f, 1.0f, 1.0f);

    //for (const char* c = text; *c; ++c) {
    //    if (FT_Load_Char(face, *c, FT_LOAD_RENDER) != 0) {
    //        // Handle error
    //        continue;
    //    }

    //    glTexImage2D(
    //        GL_TEXTURE_2D,
    //        0,
    //        GL_RED,
    //        face->glyph->bitmap.width,
    //        face->glyph->bitmap.rows,
    //        0,
    //        GL_RED,
    //        GL_UNSIGNED_BYTE,
    //        face->glyph->bitmap.buffer
    //    );

    //    float x2 = x + face->glyph->bitmap_left;
    //    float y2 = y - face->glyph->bitmap_top;
    //    float w = face->glyph->bitmap.width;
    //    float h = face->glyph->bitmap.rows;

    //    glBegin(GL_QUADS);
    //    glTexCoord2f(0, 0);
    //    glVertex2f(x2, y2);
    //    glTexCoord2f(1, 0);
    //    glVertex2f(x2 + w, y2);
    //    glTexCoord2f(1, 1);
    //    glVertex2f(x2 + w, y2 + h);
    //    glTexCoord2f(0, 1);
    //    glVertex2f(x2, y2 + h);
    //    glEnd();

    //    x += (face->glyph->advance.x >> 6);
    //    y += (face->glyph->advance.y >> 6);
    //}
}