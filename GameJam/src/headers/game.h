#pragma once

#include "core.h"
#include "player.h"
#include "map.h"
#include "menu.h"

#define TIMER 180

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
        FAIL_GAME,
        WIN_GAME,
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
    int cur_level;
    float start_time;
    int timer;
};