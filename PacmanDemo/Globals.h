#ifndef _GLOBALS_H
#define _GLOBALS_H

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
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

// enum klase

enum class Direction {
	none = 0,
	up = 1,
	left = 2,
	down = 3,
	right = 4
};

enum class EnemyState {
	chase = 0,
	scatter = 1,
	eaten = 2,
	frightened = 3,
	base = 4
};

enum class GameState {
	running = 0,
	paused = 1,
	game_over = 2,
	life_lost = 3,
	next_level = 4,
	start = 5
};

// -------------------

// strukture

struct StateInterval {
	float start;
	float end;
	EnemyState state;
};

// -------------------

// type alias definicije

template <typename T>
using Matrix = std::vector<std::vector<T>>;

template <typename T>
using Set = std::set<T>;

template <typename T>
using Vector = std::vector<T>;

using Intervals = Vector<StateInterval> ;
extern Intervals intervals;

// -------------------

// podešavanje prozora

extern const int screenWidth;
extern const int screenHeight;
extern const int windowPositionX;
extern const int windowPositionY;
extern const int backgroundXOffset;
extern const char* title;

// ------------------- 

// čvorovi

extern const float nodeSize;
extern const float nodeRenderOffset;

// ------------------- 

// matrica

constexpr int rows = 30;			  // broj redova u grafu
constexpr int columns = 31;			  // broj kolona u grafu
extern const int map[rows][columns];  // matrica grafova
extern const int dots[rows][columns]; // matrica tačaka koje igrač jede

// -------------------

// input događaji

extern const int joystickPollInterval;

// -------------------

// flagovi

extern bool toggleWireframe;
extern bool toggleRender;
extern bool toggleFrightenedMode;
extern bool hasIntervalStateChanged;
extern bool inkyExitBaseFlag;
extern bool clydeExitBaseFlag;

// -------------------

// konstante za razdaljinu

extern const float ghostDirectionChangeDistanceThreshold;
extern const float pacDirectionChangeDistanceThreshold;
extern const float turnBufferDistanceThreshold;
extern const float eatDistanceThreshold;
extern const float ghostCollisionDistanceThreshold;
extern const float clyde8NodesDistance;
extern const int pinkyTargetNodeDistance;
extern const int clydeRadiusNodeDistance;

// ------------------- 

// brzina karaktera

extern float pacSpeed;
extern float pacDotSpeed;
extern float ghostTunnelSpeed;
extern float chaseScatterSpeed;
extern float ghostDefaultspeed;
extern float eatenSpeed;
extern float frightenedSpeed;
extern float baseSpeed;
extern const float maxSpeed;
extern const float speedIncreaseCoeff;

// ------------------- 

// boje karaktera

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

// ------------------- 

// sprajtovi

extern const float normalAnimationDelay;
extern const float deathAnimatonDelay;
extern const float animationSpeed;
extern const int numberOfFramesX;
extern const int numberOfFramesY;

// ------------------- 

// ui

extern const float pacUISpacingX;
extern const float pacUIOffsetX;
extern const float pacUIOffsetY;

// ------------------- 

// indeksi čvorova

extern const int respawnNodeIndex;
extern const int baseEntranceNodeIndex;
extern const int baseEntranceBlockNodeIndex;
extern const int specialBlockNodeIndex;
extern const int leftPortalIndex;
extern const int rightPortalIndex;
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

// ------------------- 

// indeksi sprajtova

extern const int blinkyAnimRange [];
extern const int pinkyAnimRange  [];
extern const int inkyAnimRange   [];
extern const int clydeAnimRange  [];
extern const int frightenedAnimRange [];
extern const int frightenedFlashAnimRange[];
extern const int eatenAnimRange[];

// ------------------- 

// putanje do sprajtova

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

// ------------------- 

// putanje do zvučnih efekata

extern const char* pacEatSfxFilePath;
extern const char* introSfxFilePath;
extern const char* pacDieSfxFilePath;
extern const char* frightenedSfxFilePath;
extern const char* eatGhostSfxFilePath;
extern const char* extendSfxFilePath;
extern const char* siren1SfxFilePath;
extern const char* siren2SfxFilePath;
extern const char* siren3SfxFilePath;
extern const char* siren4SfxFilePath;
extern const char* siren5SfxFilePath;
extern const char* retreatingSfxFilePath;

// ------------------- 

// tajmeri

extern float elapsedTime;
extern float tempTimer;
extern float globalTimer;
extern float gameStartTimer;
extern float gameRestartTimer;
extern float lifeLostDelayTimer;
extern float nextLevelDelayTimer;
extern float clydeSwitchStateTimer;
extern float inkySwitchStateTimer;
extern float frightenedTimer;
extern float frightenedTimerThreshold;
extern float frightenedFlashTimerThreshold;
extern const float gameStartTimerThreshold;
extern const float gameRestartTimerThreshold;
extern const float lifeLostDelayTimerThreshold1;
extern const float lifeLostDelayTimerThreshold2;
extern const float nextLevelDelayTimerThreshold;
extern const float clydeSwitchStateTimerThreshold;
extern const float inkySwitchStateTimerThreshold;

// ------------------- 

// in-game promenljive

extern int health;
extern int dotCounter;
extern int currentBigDotGhostCounter;
extern int currentLevel;
extern int currentSirenSoundIndex;
extern const int bigDotValue;
extern const int smallDotValue;
extern const int maxDots;
extern const int inkyDotExitThreshold;
extern const int clydeDotExitThreshold;
extern const int maxHealth;
extern const int initialGhostEatValue;
extern const int sirensSize;
extern const int pacHealthIncreaseScoreThreshold;
extern const int ms;
extern const int heuristicCoeff;
extern bool siren1Executed;
extern bool siren2Executed;
extern bool siren3Executed;
extern bool siren4Executed;
extern bool siren5Executed;
extern bool healthIncreased;

// -------------------

// stanja

extern GameState globalGameState;
extern EnemyState globalGhostState;
extern const EnemyState blinkyInitialState;
extern const EnemyState pinkyInitialState;
extern const EnemyState inkyInitialState;
extern const EnemyState clydeInitialState;

// -------------------

// osnovne funkcije za iscrtavanje oblika

void drawCircle(float posX, float posY, float radius, float red, float green, float blue);
void drawPoint(float posX, float posY, float size, float red, float green, float blue);
void drawLine(float posX1, float posY1, float posX2, float posY2, float red, float green, float blue); 
void drawRectangle(float posX, float posY, float sizeW, float sizeH, float red, float green, float blue, int type);

// ------------------- 

std::string formatElapsedTime(float seconds);

void decreaseInterval(Intervals intervals);

#endif