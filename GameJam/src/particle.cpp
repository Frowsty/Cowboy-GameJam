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
    for (int i = 0; i < amount; i++)
    {
        olc::vf2d position = { static_cast<float>(fastrand(0, 1024)), static_cast<float>(fastrand(0, 640)) };
        particle p = { position, { position.x, static_cast<float>(fastrand(5, 20))}, { 0.f, static_cast<float>(fastrand(100, 300)) }, fastrand(100, 255)};
        particles.emplace_back(p);
    }
}

void Particles::movement()
{
    for (auto& p : particles)
    {
        if (p.position1.y >= 640)
            p.position1.y = 0 - p.position2.y;
        p.position1.y += p.velocity.y * m_pge->GetElapsedTime();
    }
}

void Particles::draw()
{
    // set draw target to render particles on their own layer
    m_pge->SetDrawTarget(layer);
    m_pge->Clear(olc::BLACK);

    // draw each particle
    for (auto& p : particles)
        m_pge->DrawLine(p.position1, { static_cast<int>(p.position2.x), static_cast<int>(p.position1.y + p.position2.y) }, olc::Pixel(p.color, p.color, p.color));

    // enable the layer we created for particles
    m_pge->EnableLayer(layer, true);

    // reset draw target after particles have been rendered
    m_pge->SetDrawTarget(nullptr);
}

void Particles::update()
{
    movement();
    draw();
}