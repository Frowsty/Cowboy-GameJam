#include "headers/game.h"

void Game::setup(olc::PixelGameEngine* pge)
{
    menu.setup(pge);
    player.m_pge = pge;
    map.m_pge = pge;
    in_game = false;
    cur_level = player.level;
    start_time = 0;
    timer = TIMER;
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
        menu.add_text({ ((float(ScreenWidth()) / 2)), ((float(ScreenHeight()) / 2) - 23) }, !in_game ? "Cowboy game jam." : "Paused.", true);
        menu.add_button({ ((float(ScreenWidth()) / 2) - 50), ((float(ScreenHeight()) / 2) - 10) }, { 100, 20 }, !in_game ? "Play" : "Continue", true, [&]()
        { 
            game_state = !in_game ? game_states::START_GAME : game_states::GAMEPLAY;
        });

        menu.add_button({ ((float(ScreenWidth()) / 2) - 50), ((float(ScreenHeight()) / 2) + 15) }, { 100, 20 }, "How to play", true, [&]()
        {
            game_state = game_states::SETTINGS_MENU;
        });

        menu.add_button({ ((float(ScreenWidth()) / 2) - 50), ((float(ScreenHeight()) / 2) + 40) }, { 100, 20 }, !in_game ? "Exit" : "Quit game", true, [&]()
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
        menu.add_text({ ((float(ScreenWidth()) / 2)), ((float(ScreenHeight()) / 2) - 23) }, "Move left: Left arrow key", true);
        menu.add_text({ ((float(ScreenWidth()) / 2)), ((float(ScreenHeight()) / 2) - 10) }, "Move right: Right arrow key", true);
        menu.add_text({ ((float(ScreenWidth()) / 2)), ((float(ScreenHeight()) / 2) + 3) }, "Jump: Up arrow key/Space", true);
        menu.add_text({ ((float(ScreenWidth()) / 2)), ((float(ScreenHeight()) / 2) + 16) }, "Double jump: Double press the jump key.", true);
        menu.add_button({ ((float(ScreenWidth()) / 2) - 50), ((float(ScreenHeight()) / 2) + 29) }, { 100, 20 }, "Back", true, [&]()
        {
            game_state = game_states::MAIN_MENU;
        });

        menu.on_input();
        menu.on_render();

        menu.reset();

        return true;

    case game_states::START_GAME:
        cur_level = player.level;
        start_time = 0;
        timer = TIMER;
        // setup the map data and load in the first map.
        switch (cur_level)
        {
        case 1: map.loadMap("./sprites/map1.json"); break;
        case 2: map.loadMap("./sprites/map1.json"); break;
        case 3: map.loadMap("./sprites/map1.json"); break;
        }

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
        // timer related stuff
        if (start_time == 0)
            start_time = GetTickCount();

        if ((GetTickCount() - start_time) >= 1000)
        {
            timer -= 1;
            start_time = GetTickCount(); // reset start_time to recount 1 second
        }
        menu.add_text({ 10, 5 }, "TIME LEFT: " + std::to_string(timer), false);

        if (timer == 0)
            game_state = game_states::EXIT_GAME;
        // end of timer

        // run the main game.
        if ((player.holding_key || player.wrong_key || player.has_correct_key) && GetTickCount() - player.pickup_time <= 1250)
            menu.add_text({ player.position.x + (player.size.x / 2), player.position.y - 10 }, player.holding_key ? "Picked up a key" : player.wrong_key ? "Wrong key" : player.has_correct_key ? "Key found" : "", true);
        
        // run most of the game logic and rendering
        map.render();
        player.update();

        if (menu.has_elements())
        {
            menu.on_render();
            menu.reset();
        }

        while (GetTickCount() - player.pickup_time <= 1250)
          return true;

        // Advance to next level if current one is different from the players level
        if (cur_level != player.level)
            game_state = game_states::START_GAME;

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