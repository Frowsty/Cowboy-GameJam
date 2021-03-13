#include "headers/game.h"

bool Game::OnUserCreate()
{    
    map.m_pge = this;
    player.m_pge = this;
    menu.setup(this);
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
        menu.add_text({ ((float(ScreenWidth()) / 2) - 50), ((float(ScreenHeight()) / 2) - 23) }, !in_game ? "Cowboy game jam." : "Paused.");
        menu.add_button({ ((float(ScreenWidth()) / 2) - 50), ((float(ScreenHeight()) / 2) - 10) }, { 100, 20 }, !in_game ? "Play" : "Continue", [&]()
        { 
            if (!in_game)
                game_state = game_states::START_GAME;
            else
                game_state = game_states::GAMEPLAY;         
        });

        menu.add_button({ ((float(ScreenWidth()) / 2) - 50), ((float(ScreenHeight()) / 2) + 15) }, { 100, 20 }, !in_game ? "Exit" : "Quit game", [&]() 
        { 
            if (!in_game)
                game_state = game_states::END_GAME;
            else           
              in_game = false;            
        });

        // update input and render.
        menu.on_input();
        menu.on_render();

        // clear the items for next frame render.
        menu.reset();

        break;

    case Game::game_states::SETTINGS_MENU:
        break;

    case Game::game_states::START_GAME:
        menu.reset();

        map.loadMap("./sprites/map1.json");

        player.create();
        player.collidable_tiles = &map.collidable_tiles;

        in_game = true;
        game_state = game_states::GAMEPLAY;

        break;

    case Game::game_states::GAMEPLAY:
        map.render();
        player.update();

        if (GetKey(olc::ESCAPE).bPressed)
          game_state = game_states::MAIN_MENU;

        break;

    case Game::game_states::END_GAME:
        return false;
    }

    return true;
}