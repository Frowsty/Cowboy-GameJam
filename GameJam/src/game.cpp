#include "headers/game.h"

void Game::setup(olc::PixelGameEngine* pge)
{
    menu.setup(pge);
    player.m_pge = pge;
    map.m_pge = pge;
    in_game = false;
    game_state = game_states::SPLASHSCREEN;
}

bool Game::OnUserCreate()
{    
    setup(this);
    return true;
}

bool Game::OnUserUpdate(float fElapsedTime)
{
    Clear(olc::BLACK);
    switch (game_state)
    {
    case game_states::SPLASHSCREEN:
        if (splash_screen.AnimateSplashScreen(fElapsedTime))
            return true;

        game_state = game_states::MAIN_MENU;
        return true;

    case game_states::MAIN_MENU:
        // push all menu elements to the vector.
        menu.add_text({ ((float(ScreenWidth()) / 2) - 50), ((float(ScreenHeight()) / 2) - 23) }, !in_game ? "Cowboy game jam." : "Paused.");
        menu.add_button({ ((float(ScreenWidth()) / 2) - 50), ((float(ScreenHeight()) / 2) - 10) }, { 100, 20 }, !in_game ? "Play" : "Continue", [&]()
        { 
            game_state = !in_game ? game_states::START_GAME : game_states::GAMEPLAY;
        });

        menu.add_button({ ((float(ScreenWidth()) / 2) - 50), ((float(ScreenHeight()) / 2) + 15) }, { 100, 20 }, "How to play", [&]()
        {
            game_state = game_states::SETTINGS_MENU;
        });

        menu.add_button({ ((float(ScreenWidth()) / 2) - 50), ((float(ScreenHeight()) / 2) + 40) }, { 100, 20 }, !in_game ? "Exit" : "Quit game", [&]() 
        { 
            if (!in_game)
                game_state = game_states::EXIT_GAME;
            else 
            {
                game_state = game_states::END_GAME;
                in_game = false;
            }
        });

        // update input and render.
        menu.on_input();
        menu.on_render();

        // clear the items for next frame render.
        menu.reset();
        return true;

    case game_states::SETTINGS_MENU:
        menu.add_text({ ((float(ScreenWidth()) / 2) - 50), ((float(ScreenHeight()) / 2) - 23) }, "Move left: Left arrow key");
        menu.add_text({ ((float(ScreenWidth()) / 2) - 50), ((float(ScreenHeight()) / 2) - 10) }, "Move right: Right arrow key");
        menu.add_text({ ((float(ScreenWidth()) / 2) - 50), ((float(ScreenHeight()) / 2) + 3) }, "Jump: Up arrow key/Space");
        menu.add_button({ ((float(ScreenWidth()) / 2) - 50), ((float(ScreenHeight()) / 2) + 16) }, { 100, 20 }, "Back", [&]()
        {
            game_state = game_states::MAIN_MENU;
        });

        menu.on_input();
        menu.on_render();

        menu.reset();

        return true;

    case game_states::START_GAME:
        // setup the map data and load in the first map.
        map.loadMap("./sprites/map1.json");

        // setup the local player and load in resources.
        player.create();
        player.collidable_tiles = &map.collidable_tiles;

        // we can start the game now.
        in_game = true;
        game_state = game_states::GAMEPLAY;
        return true;

    case game_states::END_GAME:
        game_state = game_states::MAIN_MENU;
        return true;

    case game_states::GAMEPLAY:
        // run the main game.
        map.render();
        player.update();

        // pause menu.
        if (GetKey(olc::ESCAPE).bPressed)
            game_state = game_states::MAIN_MENU;

        return true;

    case game_states::EXIT_GAME:
        // exit the game.
        return false;    
    default:
        return true;
    }
}