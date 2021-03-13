#include "headers/game.h"

bool Game::OnUserCreate()
{    
    in_game = false;
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

        // setup the menu before setting the menu state.
        menu.setup(this);
        game_state = game_states::MAIN_MENU;

        return true;

    case Game::game_states::MAIN_MENU:
        // push all menu elements to the vector.
        menu.add_text({ ((float(ScreenWidth()) / 2) - 50), ((float(ScreenHeight()) / 2) - 23) }, !in_game ? "Cowboy game jam." : "Paused.");
        menu.add_button({ ((float(ScreenWidth()) / 2) - 50), ((float(ScreenHeight()) / 2) - 10) }, { 100, 20 }, !in_game ? "Play" : "Continue", [&]()
        { 
            game_state = !in_game ? game_states::START_GAME : game_states::GAMEPLAY;
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
        return true;

    case Game::game_states::SETTINGS_MENU:
      return true;

    case Game::game_states::START_GAME:
        // setup the map data and load in the first map.
        map.m_pge = this;
        map.loadMap("./sprites/map1.json");

        // setup the local player and load in resources.
        player.m_pge = this;
        player.create();
        player.collidable_tiles = &map.collidable_tiles;

        // we can start the game now.
        in_game = true;
        game_state = game_states::GAMEPLAY;
        return true;

    case Game::game_states::GAMEPLAY:
        // run the main game.
        map.render();
        player.update();

        // pause menu.
        if (GetKey(olc::ESCAPE).bPressed)
          game_state = game_states::MAIN_MENU;

        return true;

    case Game::game_states::END_GAME:
        // exit the game.
        return false;    
    default:
        return true;
    }
}