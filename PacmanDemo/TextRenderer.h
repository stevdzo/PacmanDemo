#ifndef _TEXT_RENDERER_H
#define _TEXT_RENDERER_H

#include "Globals.h"
#include <ft2build.h>
#include <freetype/freetype.h>

class TextRenderer {

private:

    static TextRenderer* instance;
 
    FT_Library ft;
    FT_Face face;

    TextRenderer();
    TextRenderer(const char* fontPath, int fontSize);

public:

    static TextRenderer* getInstance();
    static TextRenderer* getInstance(const char* fontPath, int fontSize);

    void renderText(const char* text, float x, float y);
    //void drawStrokeText(char* p_string, int p_x, int p_y, const float p_r, const float p_g, const float p_b);  
    void drawStrokeText(char* p_string, int p_x, int p_y, const float p_r = 0.0f, const float p_g = 0.0f, const float p_b = 0.0f, const float p_scale = 2.0f);

    ~TextRenderer();
};
#endif