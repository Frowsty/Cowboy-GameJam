#pragma once

#include "core.h"
#include "player.h"
#include "map.h"
#include "menu.h"

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
        SETTINGS_MENU,
        START_GAME,
        END_GAME,
        GAMEPLAY,
        EXIT_GAME
    };

    void setup(olc::PixelGameEngine* pge);

    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;

private:
    olcPGEX_SplashScreen splash_screen;
    game_states game_state = game_states::SPLASHSCREEN;
    Player player;
    Map map;
    Menu menu;
    bool in_game;
};