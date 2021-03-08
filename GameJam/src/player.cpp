#include "headers/player.h"

Player::Player()
{
}

void Player::movement()
{
    if ((m_pge->GetKey(olc::LEFT).bPressed || m_pge->GetKey(olc::LEFT).bHeld))
    {
        std::cout << "sheesh we're pressing left key\n";
    }
    if ((m_pge->GetKey(olc::DOWN).bPressed || m_pge->GetKey(olc::DOWN).bHeld))
    {
        
    }
    if ((m_pge->GetKey(olc::UP).bPressed || m_pge->GetKey(olc::UP).bHeld))
    {
        
    }
    if ((m_pge->GetKey(olc::RIGHT).bPressed || m_pge->GetKey(olc::RIGHT).bHeld))
    {
        
    }
}

void Player::interaction()
{
    if (m_pge->GetKey(olc::SPACE).bPressed)
    {

    }
}

void Player::render()
{
    
}

void Player::update()
{
    movement();
    interaction();
    render();
}