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

enum class Direction {
	none = 0,
	up = 1,
	left = 2,
	down = 3,
	right = 4
};

enum class PlayerState {
	none = 0,
	alive = 1,
	dead = 2
};

enum class EnemyState {
	none = 0,
	chase = 1,
	scatter = 2,
	eaten = 3,
	frightened = 4,
	base = 5
};

enum class GameState {
	running = 0,
	paused = 1,
	game_over = 2,
	life_lost = 3
};

struct StateInterval {
	float start;
	float end;
	EnemyState state;
	int index;
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
extern bool frightenedModeActive;

extern bool toggleChangeMode;

extern bool gameActive;
extern bool lifeLost;
extern bool nextLevel;
extern bool hasRestarted;
extern bool hasIntervalStateChanged;

extern const float wireframeSizeX;
extern const float wireframeSizeY;

extern const float ghostDirectionChangeDistanceThreshold;
extern const float pacDirectionChangeDistanceThreshold;
extern const float turnBufferDistanceThreshold;
extern const float eatDistanceThreshold;
extern const float gameStartTimerThreshold;
extern const float frightenedTimerThreshold;
extern const float frightenedFlashTimerThreshold;

extern const float clyde8NodesDistance;

extern const float chaseScatterSpeed;
extern const float eatenSpeed;
extern const float frightenedSpeed;

extern const float pacR;
extern const float pacG;
extern const float pacB;

extern const float blinkyR;
extern const float blinkyG;
extern const float blinkyB;

extern const float pinkyR;
extern const float pinkyG;
extern const float pinkyB;

extern const float inkyR;
extern const float inkyG;
extern const float inkyB;

extern const float clydeR;
extern const float clydeG;
extern const float clydeB;

extern const float normalAnimationDelay;
extern const float deathAnimatonDelay;

extern const int respawnNodeIndex;

extern const int baseEntranceNodeIndex;

extern const int playerStartNodeIndex;

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
extern const char* eatGhostSfxFilePath;

extern float timer;

extern float tempTimer;
extern float globalTimer;
extern float gameStartTimer;

extern float frightenedTimer;

extern int dotCounter;
extern const int inkyDotExitThreshold;
extern const int clydeDotExitThreshold;

extern const int initialGhostEatValue;
extern int currentBigDotGhostCounter;

static int currentLevel = 1;

extern GameState globalGameState;

extern const EnemyState blinkyInitialState;
extern const EnemyState pinkyInitialState;
extern const EnemyState inkyInitialState;
extern const EnemyState clydeInitialState;

extern const StateInterval intervals[7];

extern void drawCircle(float posX, float posY, float radius, float red, float green, float blue);
extern void drawPoint(float posX, float posY, float size, float red, float green, float blue);
extern void drawLine(float posX1, float posY1, float posX2, float posY2, float red, float green, float blue); 
extern void drawRectangle(float posX, float posY, float sizeW, float sizeH, float red, float green, float blue, int type);

#endif