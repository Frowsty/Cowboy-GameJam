#pragma once

#include "core.h"
#include <random>
#include <deque>

struct particle
{
    olc::vf2d position1;
    olc::vf2d position2;
    olc::vf2d velocity;
    olc::Pixel color;
};

class Particles
{
public:
    olc::PixelGameEngine* m_pge;
    int layer;
    olc::vi2d boundaries;

    void set_boundaries(olc::vi2d boundaries);

    void create(int amount);

    void update();

private:
    std::deque<particle> particles;
    void movement();

    void draw();
};