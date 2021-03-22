#pragma once

#include "core.h"
#include "player.h"
#include "map.h"
#include "menu.h"
#include "particle.h"

#define TIMER 150

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
    game_states game_state;
    Player player;
    Map map;
    Menu menu;
    Particles particles;
    bool in_game;
    int cur_level;
    long long start_time;
    int timer;
};