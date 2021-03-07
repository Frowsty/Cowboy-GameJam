#define OLC_PGE_APPLICATION
#include "headers/olcPixelGameEngine.h"
#include "headers/olcPGEX_SplashScreen.h"

class Game : public olc::PixelGameEngine
{
public:
	Game()
	{
		sAppName = "Cowboy Game jam";
	}

public:

    enum game_states
    {
        SPLASHSCREEN = 0,
        MAIN_MENU,
        INTRO_MENU,
        SETTINGS_MENU,
        START_GAME,
        GAMEPLAY,
        END_GAME
    };

    // Splash screen
    olcPGEX_SplashScreen splash_screen;

    game_states game_state;

	bool OnUserCreate() override
	{
        game_state = game_states::SPLASHSCREEN;
		// Called once at the start, so create things here
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// called once per frame

        switch (game_state)
        {
        case SPLASHSCREEN:
            if (splash_screen.AnimateSplashScreen(fElapsedTime))
                return true;
            game_state = game_states::MAIN_MENU;
            break;
        case MAIN_MENU:
            for (int x = 0; x < ScreenWidth(); x++)
                for (int y = 0; y < ScreenHeight(); y++)
                    Draw(x, y, olc::Pixel(rand() % 255, rand() % 255, rand() % 255));
            break;
        
        case INTRO_MENU:
            break;
        
        case SETTINGS_MENU:
            break;

        case START_GAME:
            break;

        case GAMEPLAY:
            break;

        case END_GAME:
            break;
        }
		return true;
	}
};


int main()
{
	Game game;
	if (game.Construct(512, 256, 1, 1))
        game.Start();

	return 0;
}
