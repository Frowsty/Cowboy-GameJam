#define OLC_PGE_APPLICATION
#define OLC_PGEX_ANIMSPR

#include "headers/game.h"

int main()
{
#ifndef EMSCRIPTEN_COMPATIBLE
    #ifndef _DEBUG
		    FreeConsole();
    #endif
#endif

		Game game;
		if (game.Construct(1024, 640, 1, 1))
				game.Start();

		return 0;
}