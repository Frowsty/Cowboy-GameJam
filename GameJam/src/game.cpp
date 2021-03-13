#include "headers/game.h"

bool Game::OnUserCreate()
{
    map.m_pge = this;
    map.loadMap("./sprites/map1.json");

    player.create();
    player.collidable_tiles = &map.collidable_tiles;
    player.m_pge = this;

    menu.AddElement(new ElementButton(this, { (1024 / 2) - 50, (640 / 2) - 10 }, { 100, 20 }, "Start"));
    menu.SetCallback([&]() { game_state = game_states::GAMEPLAY; });

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
        menu.OnInput();
        menu.OnRender();

        //game_state = game_states::GAMEPLAY;
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