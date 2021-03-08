#include "headers/game.h"


bool Game::OnUserCreate()
{
    player.m_pge = this;

    return true;
}

bool Game::OnUserUpdate(float fElapsedTime)
{
    switch (game_state)
    {
    case Game::game_states::SPLASHSCREEN:
        if (splash_screen.AnimateSplashScreen(fElapsedTime))
            return true;
        game_state = game_states::MAIN_MENU;
        break;

    case Game::game_states::MAIN_MENU:
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