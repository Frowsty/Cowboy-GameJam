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

    olc::PixelGameEngine* m_pge;

    bool on_ground = false;
    bool has_correct_key = false;
    bool holding_key = false;
    bool wrong_key = false;
    bool did_interact = false;
    int interact_time = 0;
    int pickup_time = 0;
    int jump_height = 70;
    int level = 1;
    olc::vf2d jump_pos;

    float double_jump_timer;

    std::string last_animation_state;

    jump_state state;

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

    // set idle sprite
    void set_idle_sprite(int direction);

    // player input
    void movement();

    // player movement
    void jump_movement();

    // player interraction
    void interaction();

    // check collision
    bool check_collision(const Map::tile& left);

    // run collision system
    bool run_collision();

    // player rendering
    void render();

    // update player
    void update();
};
