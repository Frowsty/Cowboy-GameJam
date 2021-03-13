#include "headers/player.h"

#define GRAVITY 200
#define SPEED 150

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
    size = { 30, 30 };

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
    start_position = { 64, 600 };
    position = start_position;
    new_position = start_position;
}

void Player::set_idle_sprite(int direction)
{
    switch (direction)
    {
    case 1:
        player_sprite.SetState("idle-left");
        break;
    case 2:
        player_sprite.SetState("idle-right");
        break;
    }
}

void Player::movement()
{
    if ((m_pge->GetKey(olc::LEFT).bPressed || m_pge->GetKey(olc::LEFT).bHeld))
    {
        player_sprite.SetState("walking-left");
        new_position.x -= SPEED * m_pge->GetElapsedTime();

        if (!run_collision())
            position.x = new_position.x;
        else
            new_position.x = position.x;

        last_direction = 1;
        last_movement_tick = GetTickCount();
    }

    if ((m_pge->GetKey(olc::RIGHT).bPressed || m_pge->GetKey(olc::RIGHT).bHeld))
    {
        player_sprite.SetState("walking-right");
        new_position.x += SPEED * m_pge->GetElapsedTime();

        if (!run_collision())
            position.x = new_position.x;
        else
            new_position.x = position.x;

        last_direction = 2;
        last_movement_tick = GetTickCount();
    }

    // Run jump movement smoothly until we reach the top before we start the gravity fall again
    if (did_jump)
    {
        if ((m_pge->GetKey(olc::UP).bPressed || m_pge->GetKey(olc::SPACE).bPressed) && !double_jump)
        {
            double_jump = true;
            jump_height = 128;
        }

        new_position.y -= (SPEED * 2) * m_pge->GetElapsedTime();

        if (!run_collision())
        {
            on_ground = false;
            position.y = new_position.y;
        }
        else
            did_jump = false;

        if (std::abs(position.y - (jump_pos.y)) >= jump_height)
        {
            jump_height = 64;
            did_jump = false;
            double_jump = false;
        }
    }

    if (((m_pge->GetKey(olc::UP).bPressed || m_pge->GetKey(olc::SPACE).bPressed) || did_jump) && on_ground)
    {
        if (!did_jump)
            jump_pos = position;

        did_jump = true;
        last_movement_tick = GetTickCount();
    }

    // Constant gravity pull
    if (!did_jump)
    {
        new_position.y += GRAVITY * m_pge->GetElapsedTime();
        if (!run_collision())
        {
            on_ground = false;
            position.y = new_position.y;
        }
        else
        {
            on_ground = true;
            new_position.y = position.y;
        }
    }

    // Set idle animation after no movement has been made for x time
    if (GetTickCount() - last_movement_tick > 10)
        set_idle_sprite(last_direction);
}

void Player::interaction()
{
    
}

bool Player::check_collision(const Map::tile& left)
{
    return left.position.x + left.tile_size.x >= new_position.x
        && left.position.x < new_position.x + size.x
        && left.position.y + left.tile_size.y >= new_position.y
        && left.position.y < new_position.y + size.y;
}

bool Player::run_collision()
{
    for (auto& [name, tile] : *collidable_tiles)
    {
      if (tile->destroyed)
        continue;

#ifdef _DEBUG
        m_pge->DrawRect(c.second->position, { 32, 32 }, olc::RED);
#endif

        if (check_collision(*tile))
        {
            if (name.compare("collectable") == 0)
            {
                tile->destroyed = true;
                return false;
            }
            else if (name.compare("obstacle") == 0)
            {
                // respawn player at start
                position = start_position;
                return true;
            }
            else if (name.compare("map_terrain") == 0)
                return true;            
        }
    }

    return false;
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