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
        menu.add_text({ ((ScreenWidth() / 2.f)), ((ScreenHeight() / 2.f) - 23.f) }, !in_game ? "Cowboy game jam. 2021" : "Paused.", true);
        menu.add_button({ ((ScreenWidth() / 2.f) - 50.f), ((ScreenHeight() / 2.f) - 10.f) }, { 100, 20 }, !in_game ? "Play" : "Continue", true, [&]()
        { 
            game_state = !in_game ? game_states::START_GAME : game_states::GAMEPLAY;
        });

        menu.add_button({ ((ScreenWidth() / 2.f) - 50.f), ((ScreenHeight() / 2.f) + 15.f) }, { 100, 20 }, "How to play", true, [&]()
        {
            game_state = game_states::SETTINGS_MENU;
        });

        menu.add_button({ ((ScreenWidth() / 2.f) - 50.f), ((ScreenHeight() / 2.f) + 40.f) }, { 100, 20 }, !in_game ? "Exit" : "Quit game", true, [&]()
        { 
            if (!in_game)
                game_state = game_states::EXIT_GAME;
            else 
            {
                game_state = game_states::END_GAME;
                in_game = false;
            }
        });

        menu.add_text({ 5, 627 }, "Made by Kian and Daniel 2021", false);

        // update input and render.
        menu.on_input();
        menu.on_render();

        // clear the items for next frame render.
        menu.reset();
        return true;

    case game_states::SETTINGS_MENU:
        menu.add_text({ ((ScreenWidth() / 2.f)), ((ScreenHeight() / 2.f) - 100.f) }, "About:", true);
        menu.add_text({ ((ScreenWidth() / 2.f)), ((ScreenHeight() / 2.f) - 87.f) }, "You must collect the correct key for the chest to progress to the next area.", true);
        menu.add_text({ ((ScreenWidth() / 2.f)), ((ScreenHeight() / 2.f) - 74.f) }, "Make sure you are hastey though as you only have 150 seconds to complete this task.", true);
        menu.add_text({ ((ScreenWidth() / 2.f)), ((ScreenHeight() / 2.f) - 61.f) }, "Nots: You may only carry one key at a time.", true);

        menu.add_text({ ((ScreenWidth() / 2.f)), ((ScreenHeight() / 2.f) - 23.f) }, "Move left: Left arrow key", true);
        menu.add_text({ ((ScreenWidth() / 2.f)), ((ScreenHeight() / 2.f) - 10.f) }, "Move right: Right arrow key", true);
        menu.add_text({ ((ScreenWidth() / 2.f)), ((ScreenHeight() / 2.f) + 3.f) }, "Jump: Up arrow key/Space", true);
        menu.add_text({ ((ScreenWidth() / 2.f)), ((ScreenHeight() / 2.f) + 16.f) }, "Double jump: Double press the jump key.", true);
        menu.add_button({ ((ScreenWidth() / 2.f) - 50.f), ((ScreenHeight() / 2.f) + 29.f) }, { 100, 20 }, "Back", true, [&]()
        {
            game_state = game_states::MAIN_MENU;
        });

        menu.add_text({ 5, 627 }, "Made by Kian and Daniel 2021", false);

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
        case 2: map.loadMap("./sprites/map2.json"); break;
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
            game_state = game_states::FAIL_GAME;
        // end of timer

        // run the main game.
        if (GetTickCount() - player.pickup_time <= 1250)
            menu.add_text({ player.position.x + (player.size.x / 2), player.position.y - 10 }, player.holding_key ? player.did_interact ? "Already have a key" : "Picked up a key" : player.wrong_key ? "Wrong key" : player.has_correct_key ? "Key found" : player.holding_key && player.did_interact ? "Already have a key" : "", true);
        
        // run most of the game logic and rendering
        map.render();
        player.update();

        if (menu.has_elements())
        {
            FillRectDecal({ 7, 2 }, { GetTextSize("TIME LEFT: " + std::to_string(timer)).x + 8.f, GetTextSize("TIME LEFT: " + std::to_string(timer)).y + 5.f }, olc::BLACK);
            menu.on_render();
            menu.reset();
        }

        // Advance to next level if current one is different from the players level
        if (cur_level != player.level)
            game_state = game_states::START_GAME;

        // pause menu.
        if (GetKey(olc::ESCAPE).bPressed)
            game_state = game_states::MAIN_MENU;

        return true;

    case game_states::FAIL_GAME:
        menu.add_text({ ((ScreenWidth() / 2.f)), ((ScreenHeight() / 2.f) - 23.f) }, "Game over.", true);
        menu.add_button({ ((ScreenWidth() / 2.f) - 50.f), ((ScreenHeight() / 2.f) - 10.f) }, { 100, 20 }, "Retry", true, [&]()
        {
            game_state = game_states::START_GAME;
        });

        menu.add_button({ ((ScreenWidth() / 2.f) - 50.f), ((ScreenHeight() / 2.f) + 13.f) }, { 100, 20 }, "Main menu", true, [&]()
        {
            in_game = false;
            game_state = game_states::MAIN_MENU;
        });

        menu.on_input();
        menu.on_render();
        menu.reset();

        return true;

    case game_states::WIN_GAME:
        menu.add_text({ ((ScreenWidth() / 2.f)), ((ScreenHeight() / 2.f) - 23.f) }, "WINNER WINNER CHICKEN DINNER", true);
        menu.add_button({ ((ScreenWidth() / 2.f) - 50.f), ((ScreenHeight() / 2.f) - 10.f) }, { 100, 20 }, "Continue", true, [&]()
        {
            in_game = false;
            game_state = game_states::MAIN_MENU;
        });

        menu.on_input();
        menu.on_render();
        menu.reset();

        return true;

    case game_states::EXIT_GAME:
        // exit the game.
        return false;

    default:
        return true;
    }
}