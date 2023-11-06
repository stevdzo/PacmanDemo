#include "Globals.h"
#include "Vector2D.h"
#include "Vector3D.h"

namespace gv {

	const int screenWidth = 896;
	const int screenHeight = 992;
	const char* title = "Pacman Demo";

	const float nodeSize = 32.0f;

	const float nodeRenderOffset = nodeSize/2.0f;

	const int leftPortalIndex = 16;
	//const int rightPortalIndex = 915;
	const int rightPortalIndex = 884;

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

	bool toggleWireframe = true;

	const float directionChangeDistanceThreshold = 0.4f;
	const float turnBufferDistanceThreshold = 32.0f;
	const float eatDistanceThreshold = 8.0f;

	Vector2D defaultSize = Vector2D(32.0f, 32.0f);

	Vector3D defaultWireframeColor = Vector3D(1.0f, 1.0f, 1.0f);
	Vector3D gameObjectWireframeColor = Vector3D(0.2f, 0.8f, 0.2f);
	Vector3D nodeWireframeColor = Vector3D(0.8f, 0.2f, 0.2f);
	Vector3D edgeWireframeColor = Vector3D(0.2f, 0.8f, 0.8f);
}