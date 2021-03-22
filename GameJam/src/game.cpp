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

    // particle system
    particles.m_pge = pge;
    particles.layer = CreateLayer();
    particles.set_boundaries({ ScreenWidth(), ScreenHeight() });
    particles.create(200); // Call create last, always set boundaries before calling create

    // load default map to be drawn in main menu
    map.loadMap("./sprites/map1.json");
    menu_tint_layer = CreateLayer();
    menu_map_layer = CreateLayer();
    
}

bool Game::OnUserCreate()
{    
    setup(this);
    return true;
}

void Game::draw_menu_map(int map_layer, int tint_layer)
{
    SetDrawTarget(tint_layer);
    Clear(olc::Pixel(100, 100, 100, 150));
    EnableLayer(tint_layer, true);

    SetDrawTarget(map_layer);
    map.render();
    EnableLayer(map_layer, true);

    // reset draw layer
    SetDrawTarget(nullptr);
}

bool Game::OnUserUpdate(float fElapsedTime)
{
    Clear(olc::BLANK);
    std::string github_text = "https://github.com/Frowsty/Cowboy-GameJam";
    olc::vf2d github_size = GetTextSize(github_text);
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
                player.level = 1;
                game_state = game_states::END_GAME;
                in_game = false;
            }
        });

        menu.add_text({ 5, 627 }, "Made by Kian and Daniel 2021", false);
#ifndef EMSCRIPTEN_COMPATIBLE
        menu.add_button({ 1000 - github_size.x, 627 }, { github_size.x + 10, github_size.y }, github_text, false, [&]()
        {
            ShellExecute(0, 0, github_text.c_str(), 0, 0, SW_SHOW);
        });
#endif

        // render particles (will act as rain in the main menu)
        particles.update();

        // draw the map in the main menu
        draw_menu_map(menu_map_layer, menu_tint_layer);

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
        menu.add_text({ ((ScreenWidth() / 2.f)), ((ScreenHeight() / 2.f) + 3.f) }, "Jump: Up arrow key", true);
        menu.add_text({ ((ScreenWidth() / 2.f)), ((ScreenHeight() / 2.f) + 16.f) }, "Double jump: Jump at the peak of your first jump", true);
        menu.add_text({ ((ScreenWidth() / 2.f)), ((ScreenHeight() / 2.f) + 29.f) }, "Pick up key: Down arrow key", true);
        menu.add_button({ ((ScreenWidth() / 2.f) - 50.f), ((ScreenHeight() / 2.f) + 42.f) }, { 100, 20 }, "Back", true, [&]()
        {
            game_state = game_states::MAIN_MENU;
        });

        menu.add_text({ 5, 627 }, "Made by Kian and Daniel 2021", false);
#ifndef EMSCRIPTEN_COMPATIBLE
        menu.add_button({ 1000 - github_size.x, 627 }, { github_size.x + 10, github_size.y }, github_text, false, [&]()
            {
                ShellExecute(0, 0, github_text.c_str(), 0, 0, SW_SHOW);
            });
#endif

        // render particles (will act as rain in the main menu)
        particles.update();

        // draw the map in the main menu
        draw_menu_map(menu_map_layer, menu_tint_layer);

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
        case 3: game_state = game_states::WIN_GAME; return true;
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
            start_time = time;

        if ((time - start_time) >= 1000)
        {
            timer -= 1;
            start_time = time; // reset start_time to recount 1 second
        }

        if (timer == 0)
            game_state = game_states::FAIL_GAME;
        // end of timer

        // run the main game.
        if (player.key_state == Player::key_state::HOLDING && !player.first_pickup && (time - player.pickup_time <= 1250))
            menu.add_text({ player.position.x + (player.size.x / 2) + 10, player.position.y - 10 }, "Inventory full", true);

        if (time - player.pickup_time <= 1250)
        {
            if (player.key_state == Player::key_state::HOLDING && player.first_pickup)
                menu.add_text({ player.position.x + (player.size.x / 2), player.position.y - 10 }, "Picked up a key", true);
            else if (player.key_type == Player::key_type::WRONG && !menu.has_elements())
                menu.add_text({ player.position.x + (player.size.x / 2), player.position.y - 10 }, "Wrong key", true);
            else if (player.key_type == Player::key_type::CORRECT && !menu.has_elements())
                menu.add_text({ player.position.x + (player.size.x / 2), player.position.y - 10 }, "Found key", true);
        }
        else
            player.first_pickup = false;

        // Draw timer on screen
        menu.add_text({ 10, 5 }, "TIME LEFT: " + std::to_string(timer), false);

        // run most of the game logic and rendering
        map.render();
        player.update();

        if (menu.has_elements())
        {
            FillRectDecal({ 7, 2 }, { GetTextSize("TIME LEFT: " + std::to_string(timer)).x + 8.f, GetTextSize("TIME LEFT: " + std::to_string(timer)).y + 5.f }, olc::BLACK);
            menu.on_render();
            menu.reset();
        }

        // pause menu.
        if (GetKey(olc::ESCAPE).bPressed)
            game_state = game_states::MAIN_MENU;

        // Add slight delay before switching level
        while (time - player.pickup_time <= 1250)
            return true;

        // Advance to next level if current one is different from the players level
        if (cur_level != player.level)
            game_state = game_states::START_GAME;

        return true;

    case game_states::FAIL_GAME:
        menu.add_text({ ((ScreenWidth() / 2.f)), ((ScreenHeight() / 2.f) - 23.f) }, "Game over.", true);
        menu.add_button({ ((ScreenWidth() / 2.f) - 50.f), ((ScreenHeight() / 2.f) - 10.f) }, { 100, 20 }, "Retry", true, [&]()
        {
            player.level = 1;
            game_state = game_states::START_GAME;
        });

        menu.add_button({ ((ScreenWidth() / 2.f) - 50.f), ((ScreenHeight() / 2.f) + 13.f) }, { 100, 20 }, "Main menu", true, [&]()
        {
            player.level = 1;
            in_game = false;
            game_state = game_states::MAIN_MENU;
        });

        menu.add_text({ 5, 627 }, "Made by Kian and Daniel 2021", false);

        // render particles (will act as rain in the main menu)
        particles.update();

        // draw the map in the main menu
        draw_menu_map(menu_map_layer, menu_tint_layer);

        menu.on_input();
        menu.on_render();
        menu.reset();

        return true;

    case game_states::WIN_GAME:
        menu.add_text({ ((ScreenWidth() / 2.f)), ((ScreenHeight() / 2.f) - 23.f) }, "WINNER WINNER CHICKEN DINNER", true);
        menu.add_button({ ((ScreenWidth() / 2.f) - 50.f), ((ScreenHeight() / 2.f) - 10.f) }, { 100, 20 }, "Continue", true, [&]()
        {
            player.level = 1;
            in_game = false;
            game_state = game_states::MAIN_MENU;
        });

        menu.add_text({ 5, 627 }, "Made by Kian and Daniel 2021", false);

        // render particles (will act as rain in the main menu)
        particles.update();

        // draw the map in the main menu
        draw_menu_map(menu_map_layer, menu_tint_layer);

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