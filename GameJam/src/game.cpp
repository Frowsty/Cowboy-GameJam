#include "headers/game.h"

bool Game::OnUserCreate()
{    
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
        // push all menu elements to the vector.
        menu.add_text(this, { ((float(ScreenWidth()) / 2) - 50), ((float(ScreenHeight()) / 2) - 23) }, "Cowboy game jam.");
        menu.add_button(this, { ((float(ScreenWidth()) / 2) - 50), ((float(ScreenHeight()) / 2) - 10) }, { 100, 20 }, "Play", [&]() { game_state = game_states::START_GAME; });

        // update input and render.
        menu.on_input();
        menu.on_render();

        // clear the items for next frame render.
        menu.reset();

        break;

    case Game::game_states::INTRO_MENU:
        break;

    case Game::game_states::SETTINGS_MENU:
        break;

    case Game::game_states::START_GAME:
        map.m_pge = this;
        map.loadMap("./sprites/map1.json");

        player.create();
        player.collidable_tiles = &map.collidable_tiles;
        player.m_pge = this;

        game_state = game_states::GAMEPLAY;

        break;

    case Game::game_states::GAMEPLAY:
        menu.reset();
        map.render();
        player.update();
        break;

    case Game::game_states::END_GAME:
        break;
    }

    return true;
}