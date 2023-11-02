#ifndef _GLOBALS_H
#define _GLOBALS_H

#include <iostream>
#include <vector>

#include "glut.h"
#include "inc/fmod.hpp"

template <typename T>
using Matrix = std::vector<std::vector<T>>;

namespace gv {

	extern const int screenWidth;
	extern const int screenHeight;
	extern const char* title;

	extern const float nodeSize;
	extern const float nodeRenderOffset;

	constexpr const int rows = 29; // 28 + 1
	constexpr const int columns = 32; // 31 + 1

	extern const int map[rows][columns];

	extern bool toggleWireframe;
}
#endif