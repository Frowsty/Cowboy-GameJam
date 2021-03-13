#pragma once

#include "core.h"
#include "player.h"
#include "map.h"

class Game : public olc::PixelGameEngine
{
public:
    Game()
    {
        sAppName = "Cowboy Game jam";
    }

public:

    enum class game_states
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

    game_states game_state = game_states::MAIN_MENU; // Make sure we start at the splashscreen on startup

    bool OnUserCreate() override; // Called once at the start, so create things here

    bool OnUserUpdate(float fElapsedTime) override; // Called once per frame

private:
    Player player;
    Map map;
};