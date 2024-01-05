#ifndef _GLOBALS_H
#define _GLOBALS_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <random>
#include <thread>
#include <chrono>

#include <glut.h>

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

extern bool toggleFrightenedMode;

extern bool toggleChangeMode;

extern bool gameActive;

extern const float directionChangeDistanceThreshold;
extern const float turnBufferDistanceThreshold;
extern const float eatDistanceThreshold;	

extern const float gameStartTimerThreshold;

extern const float frightenedTimerThreshold;
extern const float frightenedFlashTimerThreshold;

extern const float chaseScatterSpeed;
extern const float eatenSpeed;
extern const float frightenedSpeed;

extern const int blinkyStartNodeIndex;
extern const int pinkyStartNodeIndex;
extern const int inkyStartNodeIndex;
extern const int clydeStartNodeIndex;

extern const int blinkyScatterNodeIndices[];
extern const int pinkyScatterNodeIndices[];
extern const int inkyScatterNodeIndices[];
extern const int clydeScatterNodeIndices[];

extern const int blinkyBaseNodeIndices[];
extern const int pinkyBaseNodeIndices[];
extern const int inkyBaseNodeIndices[];
extern const int clydeBaseNodeIndices[];

extern const int pinkyTargetNodeDistance;
extern const int clydeRadiusNodeDistance;

extern Vector2D defaultSize;

extern Vector3D defaultWireframeColor;
extern Vector3D gameObjectWireframeColor;
extern Vector3D nodeWireframeColor;
extern Vector3D edgeWireframeColor;

extern const char* pacFilePath;
extern const char* blinkyFilePath;
extern const char* pinkyFilePath;
extern const char* inkyFilePath;
extern const char* clydeFilePath;
extern const char* mazeFilePath;
extern const char* bigDotFilePath;
extern const char* smallDotFilePath;
extern const char* cherryFilePath;

extern const char* pacEatSfxFilePath;
extern const char* introSfxFilePath;
extern const char* pacDieSfxFilePath;
extern const char* frightenedSfxFilePath;

static float timer = 0.0f;

static float globalTimer = 0.0f;
static float gameStartTimer = 0.0f;

static float frightenedTimer = 0.0f;


static int currentLevel = 1;

#endif