#pragma once

#include "core.h"
#include <random>
#include <deque>

struct particle
{
    olc::vf2d position1;
    olc::vf2d position2;
    olc::vf2d velocity;
    int color;
};

class Particles
{
public:
    olc::PixelGameEngine* m_pge;
    int layer;

    void create(int amount);

private:
    std::deque<particle> particles;
    void movement();

    void draw();

public:
    void update();
};