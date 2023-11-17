#ifndef _GLOBALS_H
#define _GLOBALS_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <random>

#include "glut.h"
#include "inc/fmod.hpp"

#define PI 3.14159

class Vector2D;
class Vector3D;

enum Direction {
	none = 0,
	up = 1,
	left = 2,
	down = 3,
	right = 4
};

template <typename T>
using Matrix = std::vector<std::vector<T>>;

extern const int screenWidth;
extern const int screenHeight;
extern const char* title;

extern const float nodeSize;
extern const float nodeRenderOffset;

extern const int leftPortalIndex;
extern const int rightPortalIndex;

constexpr const int rows = 30; // 28 + 1
constexpr const int columns = 31; // 31 + 1

extern const int map[rows][columns];
extern const int dots[rows][columns];

extern bool toggleWireframe;
extern bool toggleRender;

extern const float directionChangeDistanceThreshold;
extern const float turnBufferDistanceThreshold;
extern const float eatDistanceThreshold;	

extern const int blinkyScatterNodeIndices[3];

extern Vector2D defaultSize;

extern Vector3D defaultWireframeColor;
extern Vector3D gameObjectWireframeColor;
extern Vector3D nodeWireframeColor;
extern Vector3D edgeWireframeColor;

static float timer = 0.0f;

#endif