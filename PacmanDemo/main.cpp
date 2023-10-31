#include "GlutCallbacks.h"

int main(int p_argc, char** p_argv) {
	return glutMain(p_argc, p_argv, gv::screenWidth, gv::screenHeight, gv::title);
}