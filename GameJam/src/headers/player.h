// Player class
#pragma once
#include "core.h"

class Player
{
public:
    olc::PixelGameEngine* m_pge;

public:
    Player();

    // player movement
    void movement();

    // player interraction
    void interaction();

    // player rendering
    void render();

    // update player
    void update();
};