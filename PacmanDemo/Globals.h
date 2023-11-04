#ifndef _GLOBALS_H
#define _GLOBALS_H

#include <iostream>
#include <vector>

#include "glut.h"
#include "inc/fmod.hpp"

class Vector2D;
class Vector3D;

template <typename T>
using Matrix = std::vector<std::vector<T>>;

enum Direction {
	none = 0,
	up = 1,
	down = 2,
	left = 3,
	right = 4
};

namespace gv {

	extern const int screenWidth;
	extern const int screenHeight;
	extern const char* title;

	extern const float nodeSize;
	extern const float nodeRenderOffset;

	constexpr const int rows = 29; // 28 + 1
	constexpr const int columns = 32; // 31 + 1

	extern const int map[rows][columns];
	extern const int dots[rows - 1][columns - 1];

	extern bool toggleWireframe;

	extern const float directionChangeDistanceThreshold;
	extern const float turnBufferDistanceThreshold;

	extern Vector2D defaultSize;

	extern Vector3D defaultWireframeColor;
	extern Vector3D gameObjectWireframeColor;
	extern Vector3D nodeWireframeColor;
	extern Vector3D edgeWireframeColor;
}
#endif