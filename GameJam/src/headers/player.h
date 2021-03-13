// Player class
#pragma once

#include "core.h"
#include "map.h"

class Player
{
public:
    olc::PixelGameEngine* m_pge;

    bool dead = false;
    bool did_jump = false;
    bool double_jump = false;
    bool on_ground = false;
    bool has_key = false;
    int jump_height = 64;
    int level = 1;
    olc::vf2d jump_pos;

    int last_direction = 1;
    float last_movement_tick;

    olc::vf2d start_position;
    olc::vf2d position;
    olc::vf2d new_position;
    olc::vi2d size;
    olc::AnimatedSprite player_sprite;
    olc::Renderable* spritesheet;
    std::vector<std::pair<std::string, Map::tile*>>* collidable_tiles;
public:

    // create player
    void create();

    // set idle sprite
    void set_idle_sprite(int direction);

    // player movement
    void movement();

    // player interraction
    void interaction();

    // Run collision against collidable tiles
    bool check_collision(const Map::tile& left);
    bool run_collision();

    // player rendering
    void render();

    // update player
    void update();
};
