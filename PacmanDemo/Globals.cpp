#include "Globals.h"
#include "Vector2D.h"
#include "Vector3D.h"

const int screenWidth = 896+400;
const int screenHeight = 992;
const char* title = "Pacman Demo";

const float nodeSize = 32.0f;

const float nodeRenderOffset = nodeSize / 2.0f;

const int leftPortalIndex = 16;
const int rightPortalIndex = 915;

const int _ = -1;
const int map[rows][columns] = {
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, 1, _, _, _, _, _, _, _, _, _, _, _, _, _, _},
		{_, 1, 1, 1, 1, _, _, 1, 1, 1, 1, _, _, _, _, _, 1, _, _, _, _, _, 1, 1, 1, 1, 1, 1, 1, 1, _},
		{_, 1, _, _, 1, _, _, 1, _, _, 1, _, _, _, _, _, 1, _, _, _, _, _, 1, _, _, 1, _, _, _, 1, _},
		{_, 1, _, _, 1, 1, 1, 1, _, _, 1, _, _, _, _, _, 1, _, _, _, _, _, 1, _, _, 1, _, _, _, 1, _},
		{_, 1, _, _, 1, _, _, _, _, _, 1, _, _, _, _, _, 1, _, _, _, _, _, 1, _, _, 1, _, _, _, 1, _},
		{_, 1, _, _, 1, _, _, _, _, _, 1, _, _, _, _, _, 1, _, _, _, _, _, 1, _, _, 1, _, _, _, 1, _},
		{_, 1, _, _, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, _},
		{_, 1, _, _, _, _, _, 1, _, _, 1, _, _, _, _, _, 1, _, _, _, _, _, _, _, _, 1, _, _, _, 1, _},
		{_, 1, _, _, _, _, _, 1, _, _, 1, _, _, _, _, _, 1, _, _, _, _, _, _, _, _, 1, _, _, _, 1, _},
		{_, 1, _, _, 1, 1, 1, 1, _, _, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, _, _, 1, 1, 1, 1, _, _, _, 1, _},
		{_, 1, _, _, 1, _, _, 1, _, _, 1, _, _, 1, _, _, _, _, _, 1, _, _, 1, _, _, 1, _, _, _, 1, _},
		{_, 1, _, _, 1, _, _, 1, _, _, 1, _, _, 1, _, 2, 2, 2, _, 1, _, _, 1, _, _, 1, _, _, _, 1, _},
		{_, 1, 1, 1, 1, _, _, 1, 1, 1, 1, _, _, 1, _, 0, 2, 0, _, 1, 1, 1, 1, _, _, 1, 1, 1, 1, 1, _},
		{_, 1, _, _, _, _, _, 1, _, _, _, _, _, 1, _, 2, 2, 2, 2, 2, _, _, _, _, _, 1, _, _, _, _, _},
		{_, 1, _, _, _, _, _, 1, _, _, _, _, _, 1, _, 0, 2, 0, 0, 1, _, _, _, _, _, 1, _, _, _, _, _},
		{_, 1, 1, 1, 1, _, _, 1, 1, 1, 1, _, _, 1, _, 2, 2, 2, _, 1, 1, 1, 1, _, _, 1, 1, 1, 1, 1, _},
		{_, 1, _, _, 1, _, _, 1, _, _, 1, _, _, 1, _, 0, 0, 0, _, 1, _, _, 1, _, _, 1, _, _, _, 1, _},
		{_, 1, _, _, 1, _, _, 1, _, _, 1, _, _, 1, _, _, _, _, _, 1, _, _, 1, _, _, 1, _, _, _, 1, _},
		{_, 1, _, _, 1, 1, 1, 1, _, _, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, _, _, 1, 1, 1, 1, _, _, _, 1, _},
		{_, 1, _, _, _, _, _, 1, _, _, 1, _, _, _, _, _, 1, _, _, _, _, _, _, _, _, 1, _, _, _, 1, _},
		{_, 1, _, _, _, _, _, 1, _, _, 1, _, _, _, _, _, 1, _, _, _, _, _, _, _, _, 1, _, _, _, 1, _},
		{_, 1, _, _, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, _},
		{_, 1, _, _, 1, _, _, _, _, _, 1, _, _, _, _, _, 1, _, _, _, _, _, 1, _, _, 1, _, _, _, 1, _},
		{_, 1, _, _, 1, _, _, _, _, _, 1, _, _, _, _, _, 1, _, _, _, _, _, 1, _, _, 1, _, _, _, 1, _},
		{_, 1, _, _, 1, 1, 1, 1, _, _, 1, _, _, _, _, _, 1, _, _, _, _, _, 1, _, _, 1, _, _, _, 1, _},
		{_, 1, _, _, 1, _, _, 1, _, _, 1, _, _, _, _, _, 1, _, _, _, _, _, 1, _, _, 1, _, _, _, 1, _},
		{_, 1, 1, 1, 1, _, _, 1, 1, 1, 1, _, _, _, _, _, 1, _, _, _, _, _, 1, 1, 1, 1, 1, 1, 1, 1, _},
		{_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, 1, _, _, _, _, _, _, _, _, _, _, _, _, _, _},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

const int dots[rows][columns] = {
		{_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _},
		{_, 1, 1, 1, 1, _, _, 2, 1, 1, 1, _, _, _, _, _, _, _, _, _, _, _, 1, 1, 1, 1, 1, 2, 1, 1},
		{_, 1, _, _, 1, _, _, 1, _, _, 1, _, _, _, _, _, _, _, _, _, _, _, 1, _, _, 1, _, _, _, 1},
		{_, 1, _, _, 1, 1, 1, 1, _, _, 1, _, _, _, _, _, _, _, _, _, _, _, 1, _, _, 1, _, _, _, 1},
		{_, 1, _, _, 1, _, _, _, _, _, 1, _, _, _, _, _, _, _, _, _, _, _, 1, _, _, 1, _, _, _, 1},
		{_, 1, _, _, 1, _, _, _, _, _, 1, _, _, _, _, _, _, _, _, _, _, _, 1, _, _, 1, _, _, _, 1},
		{_, 1, _, _, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{_, 1, _, _, _, _, _, 1, _, _, 1, _, _, _, _, _, _, _, _, _, _, _, _, _, _, 1, _, _, _, 1},
		{_, 1, _, _, _, _, _, 1, _, _, 1, _, _, _, _, _, _, _, _, _, _, _, _, _, _, 1, _, _, _, 1},
		{_, 1, _, _, 1, 1, 1, 1, _, _, 1, _, _, _, _, _, _, _, _, _, _, _, 1, 1, 1, 1, _, _, _, 1},
		{_, 1, _, _, 1, _, _, 1, _, _, 1, _, _, _, _, _, _, _, _, _, _, _, 1, _, _, 1, _, _, _, 1},
		{_, 1, _, _, 1, _, _, 1, _, _, 1, _, _, _, _, _, _, _, _, _, _, _, 1, _, _, 1, _, _, _, 1},
		{_, 1, 1, 1, 1, _, _, 1, 1, 1, 1, _, _, _, _, _, _, _, _, _, _, _, 1, _, _, 1, 1, 1, 1, 1},
		{_, 1, _, _, _, _, _, 1, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, 1, _, _, _, _},
		{_, 1, _, _, _, _, _, 1, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, 1, _, _, _, _},
		{_, 1, 1, 1, 1, _, _, 1, 1, 1, 1, _, _, _, _, _, _, _, _, _, _, _, 1, _, _, 1, 1, 1, 1, 1},
		{_, 1, _, _, 1, _, _, 1, _, _, 1, _, _, _, _, _, _, _, _, _, _, _, 1, _, _, 1, _, _, _, 1},
		{_, 1, _, _, 1, _, _, 1, _, _, 1, _, _, _, _, _, _, _, _, _, _, _, 1, _, _, 1, _, _, _, 1},
		{_, 1, _, _, 1, 1, 1, 1, _, _, 1, _, _, _, _, _, _, _, _, _, _, _, 1, 1, 1, 1, _, _, _, 1},
		{_, 1, _, _, _, _, _, 1, _, _, 1, _, _, _, _, _, _, _, _, _, _, _, _, _, _, 1, _, _, _, 1},
		{_, 1, _, _, _, _, _, 1, _, _, 1, _, _, _, _, _, _, _, _, _, _, _, _, _, _, 1, _, _, _, 1},
		{_, 1, _, _, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{_, 1, _, _, 1, _, _, _, _, _, 1, _, _, _, _, _, _, _, _, _, _, _, 1, _, _, 1, _, _, _, 1},
		{_, 1, _, _, 1, _, _, _, _, _, 1, _, _, _, _, _, _, _, _, _, _, _, 1, _, _, 1, _, _, _, 1},
		{_, 1, _, _, 1, 1, 1, 1, _, _, 1, _, _, _, _, _, _, _, _, _, _, _, 1, _, _, 1, _, _, _, 1},
		{_, 1, _, _, 1, _, _, 1, _, _, 1, _, _, _, _, _, _, _, _, _, _, _, 1, _, _, 1, _, _, _, 1},
		{_, 1, 1, 1, 1, _, _, 2, 1, 1, 1, _, _, _, _, _, _, _, _, _, _, _, 1, 1, 1, 1, 1, 2, 1, 1}
};

bool toggleWireframe = false;
bool toggleRender = !toggleWireframe;

bool toggleFrightenedMode = false;
bool frightenedModeActive = false;

bool toggleChangeMode = false;

bool gameActive = false;
bool lifeLost = false;
bool nextLevel = false;

const float ghostDirectionChangeDistanceThreshold = 0.8f; 
const float pacDirectionChangeDistanceThreshold = 0.8f;
const float turnBufferDistanceThreshold = 200.0f;
const float eatDistanceThreshold = 8.0f;

const float gameStartTimerThreshold = 6.0f;

const float frightenedTimerThreshold = 6.0f;
const float frightenedFlashTimerThreshold = 4.0f;

const int respawnNodeIndex = 450;

const float chaseScatterSpeed = 88.0f * 3.0f;;
const float eatenSpeed = 300.0f;
const float frightenedSpeed = 70.0f;

const int baseEntranceNodeIndex = 453;

const int playerStartNodeIndex = 441;

const int blinkyStartNodeIndex = 453;
const int pinkyStartNodeIndex  = 450;
const int inkyStartNodeIndex   = 388;
const int clydeStartNodeIndex  = 512;

const int blinkyScatterNodeIndices [] = { 711, 864, 738 };
const int pinkyScatterNodeIndices  [] = { 246, 89 , 211 };
const int inkyScatterNodeIndices   [] = { 688, 714, 498 };
const int clydeScatterNodeIndices  [] = { 223, 187, 405 };

const int blinkyBaseNodeIndices    [] = { 0,   0,   0   };
const int pinkyBaseNodeIndices     [] = { 449, 451, 453 };
const int inkyBaseNodeIndices      [] = { 389, 387, 453 };
const int clydeBaseNodeIndices     [] = { 513, 511, 453 };

extern const int pinkyTargetNodeDistance = 4;
extern const int clydeRadiusNodeDistance = 8;

Vector2D defaultSize = Vector2D(32.0f, 32.0f);

Vector3D defaultWireframeColor = Vector3D(1.0f, 1.0f, 1.0f);
Vector3D gameObjectWireframeColor = Vector3D(0.2f, 0.8f, 0.2f);
Vector3D nodeWireframeColor = Vector3D(0.8f, 0.2f, 0.2f);
Vector3D edgeWireframeColor = Vector3D(0.2f, 0.8f, 0.8f);

const char* pacFilePath       = "resources/images/pacman.png";
const char* blinkyFilePath    = "resources/images/ghost_blinky.png";
const char* pinkyFilePath     = "resources/images/ghost_pinky.png";
const char* inkyFilePath      = "resources/images/ghost_inky.png";
const char* clydeFilePath     = "resources/images/ghost_clyde.png";
const char* mazeFilePath      = "resources/images/mazelevel.png";
const char* bigDotFilePath    = "resources/images/dot_big.png";
const char* smallDotFilePath  = "resources/images/dot_small.png";
const char* cherryFilePath    = "resources/images/cherry.png";

const char* pacEatSfxFilePath = "resources/sounds/credit.wav";
const char* introSfxFilePath  = "resources/sounds/intro_sound.wav";
const char* pacDieSfxFilePath = "resources/sounds/die.mp3";
const char* frightenedSfxFilePath = "resources/sounds/frightened.wav";
const char* eatGhostSfxFilePath = "resources/sounds/eat_ghost.wav";

float timer = 0.0f;
float tempTimer = 0.0f;
float globalTimer = 0.0f;
float gameStartTimer = 0.0f;
float frightenedTimer = 0.0f;

const int initialGhostEatValue = 200;
int currentBigDotGhostCounter = 1;

GameState globalGameState = GameState::paused;

const EnemyState blinkyInitialState = EnemyState::scatter;
const EnemyState pinkyInitialState = EnemyState::base;
const EnemyState inkyInitialState = EnemyState::base;
const EnemyState clydeInitialState = EnemyState::base;

const StateInterval intervals[] = {
		{ 7.0f, 27.0f, EnemyState::chase },
		{ 27.0f, 34.0f, EnemyState::scatter },
		{ 34.0f, 54.0f, EnemyState::chase },
		{ 54.0f, 59.0f, EnemyState::scatter },
		{ 59.0f, 79.0f, EnemyState::chase },
		{ 79.0f, 84.0f, EnemyState::scatter },
		{ 84.0f, FLT_MAX, EnemyState::chase }
};