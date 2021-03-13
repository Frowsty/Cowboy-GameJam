#include "headers/game.h"

bool Game::OnUserCreate()
{
    map.m_pge = this;
    map.loadMap("./sprites/map1.json");

    player.create();
    player.collidable_tiles = &map.collidable_tiles;
    player.m_pge = this;



    return true;
}

bool Game::OnUserUpdate(float fElapsedTime)
{
    Game::Clear(olc::BLACK);
    switch (game_state)
    {
    case Game::game_states::SPLASHSCREEN:
        if (splash_screen.AnimateSplashScreen(fElapsedTime))
            return true;

        game_state = game_states::MAIN_MENU;
        break;

    case Game::game_states::MAIN_MENU:
        // put these in OnUserCreate for static menu.
        menu.AddButton(this, { 10, 10 }, { 100, 20 }, "Start");
        menu.SetCallback([&]() { game_state = game_states::GAMEPLAY; });

        // always keep these here!
        menu.OnInput();
        menu.OnRender();

        // dont call this for static menu.
        menu.Reset();

        break;

    case Game::game_states::INTRO_MENU:
        break;

    case Game::game_states::SETTINGS_MENU:
        break;

    case Game::game_states::START_GAME:
        break;

    case Game::game_states::GAMEPLAY:
        map.render();
        player.update();
        break;

    case Game::game_states::END_GAME:
        break;
    }

    return true;
}