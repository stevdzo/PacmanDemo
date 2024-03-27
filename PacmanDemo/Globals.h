#ifndef _GLOBALS_H
#define _GLOBALS_H

#include <iostream>
#include <vector>
#include <map>
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
	life_lost = 3,
	next_level = 4,
	intro = 5
};

struct StateInterval {
	float start;
	float end;
	EnemyState state;
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

extern const int numberOfFramesX;
extern const int numberOfFramesY;

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

extern bool inkyExitBaseFlag;
extern bool clydeExitBaseFlag;

extern bool everyGhostCloseToNode;

extern const float wireframeSizeX;
extern const float wireframeSizeY;

extern const float ghostDirectionChangeDistanceThreshold;
extern const float pacDirectionChangeDistanceThreshold;
extern const float turnBufferDistanceThreshold;
extern const float eatDistanceThreshold;
extern const float ghostCollisionDistanceThreshold;

extern const float clyde8NodesDistance;

extern float pacSpeed;
extern float pacDotSpeed;
extern float ghostTunnelSpeed;
extern float chaseScatterSpeed;
extern float eatenSpeed;
extern float frightenedSpeed;
extern float baseSpeed;
extern const float maxSpeed;
extern const float speedIncreaseCoeff;

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

extern const int baseEntranceBlockNodeIndex;
extern const int specialBlockNodeIndex;

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

extern const int blinkyAnimRange [];
extern const int pinkyAnimRange  [];
extern const int inkyAnimRange   [];
extern const int clydeAnimRange  [];

extern const int frightenedAnimRange [];
extern const int frightenedFlashAnimRange[];
extern const int eatenAnimRange[];

extern const char* assetsFilePath;
extern const char* pacFilePath;
extern const char* blinkyFilePath;
extern const char* pinkyFilePath;
extern const char* inkyFilePath;
extern const char* clydeFilePath;
extern const char* mazeFilePath;
extern const char* mazeFlash1FilePath;
extern const char* mazeFlash2FilePath;
extern const char* bigDotFilePath;
extern const char* smallDotFilePath;
extern const char* cherryFilePath;

extern const char* pacEatSfxFilePath;
extern const char* introSfxFilePath;
extern const char* pacDieSfxFilePath;
extern const char* frightenedSfxFilePath;
extern const char* eatGhostSfxFilePath;
extern const char* siren1SfxFilePath;
extern const char* siren2SfxFilePath;
extern const char* siren3SfxFilePath;
extern const char* siren4SfxFilePath;
extern const char* siren5SfxFilePath;
extern const char* retreatingSfxFilePath;

extern float tempTimer;
extern float globalTimer;
extern float gameStartTimer;
extern float gameRestartTimer;
extern float lifeLostDelayTimer;
extern float nextLevelDelayTimer;
extern float clydeSwitchStateTimer;
extern float inkySwitchStateTimer;
extern float frightenedTimer;

extern const float gameStartTimerThreshold;
extern const float gameRestartTimerThreshold;
extern const float lifeLostDelayTimerThreshold1;
extern const float lifeLostDelayTimerThreshold2;
extern const float nextLevelDelayTimerThreshold;
extern const float clydeSwitchStateTimerThreshold;
extern const float inkySwitchStateTimerThreshold;
extern const float frightenedTimerThreshold;
extern const float frightenedFlashTimerThreshold;

extern int eatenCount;
extern int dotCounter;
extern const int maxDots;
extern const int inkyDotExitThreshold;
extern const int clydeDotExitThreshold;

extern const int initialGhostEatValue;
extern int currentBigDotGhostCounter;

extern int currentLevel;
extern int currentSirenSoundIndex;
extern const int sirensSize;

extern bool siren1Executed;
extern bool siren2Executed;
extern bool siren3Executed;
extern bool siren4Executed;
extern bool siren5Executed;

extern GameState globalGameState;

extern EnemyState globalGhostState;

extern const EnemyState blinkyInitialState;
extern const EnemyState pinkyInitialState;
extern const EnemyState inkyInitialState;
extern const EnemyState clydeInitialState;

extern StateInterval intervals[7];

void drawCircle(float posX, float posY, float radius, float red, float green, float blue);
void drawPoint(float posX, float posY, float size, float red, float green, float blue);
void drawLine(float posX1, float posY1, float posX2, float posY2, float red, float green, float blue); 
void drawRectangle(float posX, float posY, float sizeW, float sizeH, float red, float green, float blue, int type);

#endif