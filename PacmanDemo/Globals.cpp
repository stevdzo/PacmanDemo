#include "Globals.h"

namespace gv {

	const int screenWidth = 896;
	const int screenHeight = 992;
	const char* title = "Pacman Demo";

	const float nodeSize = 32.0f;

	const float nodeRenderOffset = nodeSize/2.0f;

	const int _ = -1;
	const int map[rows][columns] = {
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
		{_, 1, _, _, 1, _, _, 1, _, _, 1, _, _, 1, _, 1, 1, 1, _, 1, _, _, 1, _, _, 1, _, _, _, 1, _},
		{_, 1, 1, 1, 1, _, _, 1, 1, 1, 1, _, _, 1, _, 1, 1, 1, _, 1, 1, 1, 1, _, _, 1, 1, 1, 1, 1, _},
		{_, 1, _, _, _, _, _, 1, _, _, _, _, _, 1, _, 1, 1, 1, 1, 1, _, _, _, _, _, 1, _, _, _, _, _},
		{_, 1, _, _, _, _, _, 1, _, _, _, _, _, 1, _, 1, 1, 1, 1, 1, _, _, _, _, _, 1, _, _, _, _, _},
		{_, 1, 1, 1, 1, _, _, 1, 1, 1, 1, _, _, 1, _, 1, 1, 1, _, 1, 1, 1, 1, _, _, 1, 1, 1, 1, 1, _},
		{_, 1, _, _, 1, _, _, 1, _, _, 1, _, _, 1, _, 1, 1, 1, _, 1, _, _, 1, _, _, 1, _, _, _, 1, _},
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
		{_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, 1, _, _, _, _, _, _, _, _, _, _, _, _, _, _}
	};

	bool toggleWireframe = false;
}