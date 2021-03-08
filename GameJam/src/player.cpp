#include "headers/player.h"

Player::Player() { }

void Player::movement()
{
    if ((m_pge->GetKey(olc::LEFT).bPressed || m_pge->GetKey(olc::LEFT).bHeld))
    {
        std::cout << "left\n";
    }

    if ((m_pge->GetKey(olc::RIGHT).bPressed || m_pge->GetKey(olc::RIGHT).bHeld))
    {
      std::cout << "down\n";
    }

    if ((m_pge->GetKey(olc::UP).bPressed || m_pge->GetKey(olc::UP).bHeld))
    {
      std::cout << "lup\n";
    }

    if ((m_pge->GetKey(olc::DOWN).bPressed || m_pge->GetKey(olc::DOWN).bHeld))
    {
      std::cout << "down\n";
    }

    if (m_pge->GetKey(olc::SPACE).bPressed)
    {
      std::cout << "interact\n";
    }
}

void Player::interaction()
{
  
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