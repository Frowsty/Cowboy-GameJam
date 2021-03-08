#define OLC_PGE_APPLICATION
#define OLC_PGEX_ANIMSPR
#include "headers/game.h"

int main()
{
#ifndef _DEBUG
	FreeConsole();
#endif

	Game game;
	if (game.Construct(512, 256, 1, 1))
        game.Start();

	return 0;
}
