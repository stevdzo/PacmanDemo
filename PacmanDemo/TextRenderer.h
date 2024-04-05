#ifndef _TEXT_RENDERER_H
#define _TEXT_RENDERER_H

#include "Globals.h"

class TextRenderer {

private:

    static TextRenderer* instance;
 
    TextRenderer();

public:

    static TextRenderer* getInstance();

    void drawStrokeText(char* p_string, int p_x, int p_y, const float p_r = 0.0f, const float p_g = 0.0f, const float p_b = 0.0f, const float p_scale = 2.0f);

    ~TextRenderer();
};
#endif