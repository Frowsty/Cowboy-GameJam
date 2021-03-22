#include "headers/particle.h"

int fastrand(int low, int high)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(low, high);

    return dis(gen);
}

void Particles::create(int amount)
{
    particles.resize(amount);
    for (auto& p : particles)
    {
        p.position1 = { static_cast<float>(fastrand(0, boundaries.x)), static_cast<float>(fastrand(0, boundaries.y)) };
        p.position2 = { p.position1.x, static_cast<float>(fastrand(5, 20)) };
        p.velocity = { 0.f, static_cast<float>(fastrand(150, 300)) };
        uint8_t g_color = fastrand(100, 155);
        p.color = { 0, g_color, static_cast<uint8_t>(g_color + 100), 255};
    }
}

void Particles::movement()
{
    for (auto& p : particles)
    {
        if (p.position1.y >= boundaries.y)
            p.position1.y = 0 - p.position2.y;
        p.position1.y += p.velocity.y * m_pge->GetElapsedTime();
    }
}

void Particles::draw()
{
    // set draw target to render particles on their own layer

    if (layer)
    {
        m_pge->SetDrawTarget(layer);
        m_pge->Clear(olc::BLANK);
    }
    // draw each particle
    for (auto& p : particles)
        m_pge->DrawLine(p.position1, { static_cast<int>(p.position2.x), static_cast<int>(p.position1.y + p.position2.y) }, p.color);

    if (layer)
    {
        // enable the layer we created for particles
        m_pge->EnableLayer(layer, true);

        // reset draw target after particles have been rendered
        m_pge->SetDrawTarget(nullptr);
    }
}

void Particles::set_boundaries(olc::vi2d b)
{
    boundaries = b;
}

void Particles::update()
{
    movement();
    draw();
}