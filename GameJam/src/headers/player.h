// Player class
#pragma once

#include "core.h"

class Player
{
public:
    olc::PixelGameEngine* m_pge;

    olc::vf2d position;
    olc::AnimatedSprite player_sprite;
    olc::Renderable* spritesheet;

public:
    Player();

    // create player
    void create();

    // player movement
    void movement();

    // player interraction
    void interaction();

    // player rendering
    void render();

    // update player
    void update();
};