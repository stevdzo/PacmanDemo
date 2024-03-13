#include "Globals.h"
#include "Vector2D.h"
#include "Vector3D.h"

const int screenWidth = 896+400;
const int screenHeight = 992;
const char* title = "Pacman Demo";

const float nodeSize = 32.0f;

const float nodeRenderOffset = nodeSize / 2.0f;

const int leftPortalIndex = 16; // 47
const int rightPortalIndex = 915; // 884

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

const int numberOfFramesX = 14;
const int numberOfFramesY = 8;

bool toggleWireframe = false;
bool toggleRender = !toggleWireframe;

bool toggleFrightenedMode = false;
bool frightenedModeActive = false;

bool toggleChangeMode = false;

bool gameActive = false;
bool lifeLost = false;
bool nextLevel = false;
bool hasRestarted = false;
bool hasIntervalStateChanged = false;

bool inkyExitBaseFlag = false;
bool clydeExitBaseFlag = false;

const float wireframeSizeX = nodeSize;
const float wireframeSizeY = nodeSize;

const float ghostDirectionChangeDistanceThreshold = 2.0f; 
const float pacDirectionChangeDistanceThreshold = 10.0f; // 0.8
const float turnBufferDistanceThreshold = 200.0f;
const float eatDistanceThreshold = 8.0f;

const float clyde8NodesDistance = 256.0f;

const int respawnNodeIndex = 450; 

const float chaseScatterSpeed = 290.0f;
//const float chaseScatterSpeed = 50;
const float eatenSpeed = 400.0f;
const float frightenedSpeed = 170.0f;
//const float frightenedSpeed = 50.0f;
const float baseSpeed = 100.0f;

const float pacR = 254.0f/255.0f;
const float pacG = 255.0f/255.0f;
const float pacB = 0.0f/255.0f;

const float blinkyR = 255.0f/255.0f;
const float blinkyG = 0.0f/255.0f;
const float blinkyB = 0.0f/255.0f;

const float pinkyR = 255.0f/255.0f;
const float pinkyG = 184.0f/255.0f;
const float pinkyB = 255.0f/255.0f;

const float inkyR = 0.0f/255.0f;
const float inkyG = 255.0f/255.0f;
const float inkyB = 255.0f/255.0f;

const float clydeR = 255.0f/255.0f;
const float clydeG = 184.0f/255.0f;
const float clydeB = 82.0f/255.0f;

const float normalAnimationDelay = 0.15f;
const float deathAnimatonDelay = 0.4;

const int baseEntranceNodeIndex = 453; // čvor ispred ulaza u bazu
const int baseEntranceBlockNodeIndex = 452; // čvor kojim blokiram ulaz u bazu

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

const int pinkyTargetNodeDistance = 4;
const int clydeRadiusNodeDistance = 8;

const int blinkyAnimRange [8] = { 56, 57,  58,  59,  60,  61,  62, 63  };
const int pinkyAnimRange  [8] = { 70, 71,  72,  73,  74,  75,  76, 77  };
const int inkyAnimRange   [8] = { 84, 85,  86,  87,  88,  89,  90, 91  };
const int clydeAnimRange  [8] = { 98, 99, 100, 101, 102, 103, 104, 105 };

const int frightenedAnimRange      [2] = { 64, 65 };
const int frightenedFlashAnimRange [2] = { 66, 67 };
const int eatenAnimRange		   [4] = { 78, 79, 80, 81 };

const char* assetsFilePath       = "resources/images/sprite_sheet_2.png";
const char* pacFilePath       = "resources/images/sprite_sheet_2.png";
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

float tempTimer = 0.0f;
float globalTimer = 0.0f;
float gameStartTimer = 0.0f;
float lifeLostDelayTimer = 0.0f;
float nextLevelDelayTimer = 0.0f;
float clydeSwitchStateTimer = 0.0f;
float inkySwitchStateTimer = 0.0f;
float frightenedTimer = 0.0f;

const float gameStartTimerThreshold = 6.0f;
const float lifeLostDelayTimerThreshold = 3.0f;
const float nextLevelDelayTimerThreshold = 3.0f;
const float clydeSwitchStateTimerThreshold = 2.0f;
const float inkySwitchStateTimerThreshold = 2.0f;
const float frightenedTimerThreshold = 7.0f;
const float frightenedFlashTimerThreshold = 4.0f;

int dotCounter = 0;
const int inkyDotExitThreshold = 30;
const int clydeDotExitThreshold = 70;

const int initialGhostEatValue = 200;
int currentBigDotGhostCounter = 1;

GameState globalGameState = GameState::paused;

EnemyState globalGhostState = EnemyState::scatter;

const EnemyState blinkyInitialState = EnemyState::scatter;
const EnemyState pinkyInitialState = EnemyState::base;
const EnemyState inkyInitialState = EnemyState::base;
const EnemyState clydeInitialState = EnemyState::base;

StateInterval intervals[] = {
	{7.0f, 27.0f, EnemyState::chase},
	{27.0f, 34.0f, EnemyState::scatter},
	{34.0f, 54.0f, EnemyState::chase},
	{54.0f, 59.0f, EnemyState::scatter},
	{59.0f, 79.0f, EnemyState::chase},
	{79.0f, 84.0f, EnemyState::scatter},
	{84.0f, FLT_MAX, EnemyState::chase}
};

void drawCircle(float posX, float posY, float radius, float red, float green, float blue) {
	int i, n = 360;
	float theta;
	glColor3f(red, green, blue);
	glBegin(GL_LINE_LOOP);
	for (i = 0; i < n; i++) {
		theta = 2 * PI * i / n;
		glVertex2f(posX + radius * cos(theta), posY + radius * sin(theta));
	}
	glEnd();
}

void drawPoint(float posX, float posY, float size, float red, float green, float blue) {
	glPointSize(size);
	glColor3f(red, green, blue);
	glBegin(GL_POINTS);
	glVertex2f(posX, posY);
	glEnd();
}

void drawLine(float posX1, float posY1, float posX2, float posY2, float red, float green, float blue) {
	glColor3f(red, green, blue);
	glBegin(GL_LINES);
	glVertex2f(posX1, posY1);
	glVertex2f(posX2, posY2);
	glEnd();
}

void drawRectangle(float posX, float posY, float sizeW, float sizeH, float red, float green, float blue, int type) {
	glColor3f(red, green, blue);
	glBegin(type);
	glVertex2f(posX - sizeW/2, posY - sizeH/2);
	glVertex2f(posX + sizeW/2, posY - sizeH/2);
	glVertex2f(posX + sizeW/2, posY + sizeH/2);
	glVertex2f(posX - sizeW/2, posY + sizeH/2);
	glEnd();
}