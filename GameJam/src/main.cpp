#define OLC_PGE_APPLICATION
#include "headers/game.h"


int main()
{
	Game game;
	if (game.Construct(512, 256, 1, 1))
        game.Start();

	return 0;
}
