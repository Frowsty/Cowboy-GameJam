#include "headers/game.h"

bool Game::OnUserCreate()
{
    map.m_pge = this;
    map.loadMap("./sprites/test_map.json");

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
        map.render();
        player.update();
        break;

    case Game::game_states::INTRO_MENU:
        break;

    case Game::game_states::SETTINGS_MENU:
        break;

    case Game::game_states::START_GAME:
        break;

    case Game::game_states::GAMEPLAY:
        break;

    case Game::game_states::END_GAME:
        break;
    }

    return true;
}