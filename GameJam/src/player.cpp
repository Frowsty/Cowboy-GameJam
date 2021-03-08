#include "headers/player.h"

Player::Player() { }

void Player::create()
{
    // set animation type and mode
    player_sprite.type = olc::AnimatedSprite::SPRITE_TYPE::DECAL;
    player_sprite.mode = olc::AnimatedSprite::SPRITE_MODE::SINGLE;
    spritesheet = new olc::Renderable();

    // load spritesheet
    spritesheet->Load("./sprites/character.png");
    player_sprite.spriteSheet = spritesheet;
    player_sprite.SetSpriteSize({ 32, 32 });

    // Add different animation states
    player_sprite.AddState("idle-down", { olc::vi2d(32, 0) });
    player_sprite.AddState("walking-down", { olc::vi2d(0, 0), olc::vi2d(64, 0) });
    player_sprite.AddState("idle-left", { olc::vi2d(32, 32) });
    player_sprite.AddState("walking-left", { olc::vi2d(0, 32), olc::vi2d(64, 32) });
    player_sprite.AddState("idle-right", { olc::vi2d(32, 64) });
    player_sprite.AddState("walking-right", { olc::vi2d(0, 64), olc::vi2d(64, 64) });
    player_sprite.AddState("idle-up", { olc::vi2d(32, 96) });
    player_sprite.AddState("walking-up", { olc::vi2d(0, 96), olc::vi2d(64, 96) });

    player_sprite.SetState("idle-down");

    // initialize a starting position
    position = { 10, 10 };
}

void Player::movement()
{
    if ((m_pge->GetKey(olc::LEFT).bPressed || m_pge->GetKey(olc::LEFT).bHeld))
    {
        player_sprite.SetState("walking-left");
        position.x -= 150 * m_pge->GetElapsedTime();
        //std::cout << "left\n";
    }

    if ((m_pge->GetKey(olc::RIGHT).bPressed || m_pge->GetKey(olc::RIGHT).bHeld))
    {
        player_sprite.SetState("walking-right");
        position.x += 150 * m_pge->GetElapsedTime();
        //std::cout << "right\n";
    }

    if ((m_pge->GetKey(olc::UP).bPressed || m_pge->GetKey(olc::UP).bHeld))
    {
        player_sprite.SetState("walking-up");
        position.y -= 150 * m_pge->GetElapsedTime();
        //std::cout << "up\n";
    }

    if ((m_pge->GetKey(olc::DOWN).bPressed || m_pge->GetKey(olc::DOWN).bHeld))
    {
        player_sprite.SetState("walking-down");
        position.y += 150 * m_pge->GetElapsedTime();
        //std::cout << "down\n";
    }
}

void Player::interaction()
{
  if (m_pge->GetKey(olc::SPACE).bPressed)
  {
    std::cout << "interact\n";
  }
}

void Player::render()
{
    player_sprite.Draw(m_pge->GetElapsedTime(), position);
}

void Player::update()
{
    movement();
    interaction();
    render();
}