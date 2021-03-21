// Player class
#pragma once

#include "core.h"
#include "map.h"

class Player
{
public:
    enum class jump_state
    {
        NONE = 0,
        JUMPING
    };

    enum class key_state
    {
        NONE = 0,
        HOLDING
    };

    enum class key_type
    {
        WRONG = 0,
        CORRECT 
    };

    olc::PixelGameEngine* m_pge;

    bool debug_mode = false;

    bool on_ground = false;
    bool first_pickup = false;
    float gravity_multiplier = 1.0f;
    int interact_time = 0;
    int pickup_time = 0;
    int jump_height = 70;
    int level = 1;
    olc::vf2d jump_pos;

    float double_jump_timer;
    bool double_jump = false;

    std::string last_animation_state;

    jump_state state;
    key_state key_state;
    key_type key_type;

    olc::vf2d start_position;
    olc::vf2d position;
    olc::vf2d old_position;
    olc::vi2d size;
    olc::AnimatedSprite player_sprite;
    olc::Renderable* spritesheet;
    std::vector<std::pair<std::string, Map::tile*>>* collidable_tiles;
public:

    // create player
    void create();

    // player input
    void movement();

    // player movement
    void jump_movement();

    // player interraction
    void interaction();

    // check collision
    bool check_collision(const Map::tile& left);

    // run collision system
    bool run_collision(bool interaction);

    // player rendering
    void render();

    // update player
    void update();
};
