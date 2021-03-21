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
    size = { 25, 32 };

    // Add different animation states
    player_sprite.AddState("idle-left", { olc::vi2d(32, 32) });
    player_sprite.AddState("walking-left", { olc::vi2d(0, 32), olc::vi2d(64, 32) });
    player_sprite.AddState("idle-right", { olc::vi2d(32, 64) });
    player_sprite.AddState("walking-right", { olc::vi2d(0, 64), olc::vi2d(64, 64) });

    player_sprite.SetState("idle-left");
    last_animation_state = player_sprite.GetState();

    // initialize a starting position
    start_position = { 64, 600 };
    position = start_position;
    old_position = start_position;

    key_state = key_state::NONE;
}

void Player::jump_movement()
{
    switch (state)
    {
    case jump_state::JUMPING:
        old_position.y = position.y;
        position.y -= (SPEED * 2) * m_pge->GetElapsedTime();

        jump_height = 70;

        if (std::abs(position.y - jump_pos.y) > jump_height || run_collision(false))
        {
            position.y = old_position.y;
            double_jump_timer = GetTickCount();
            state = jump_state::NONE;
        }
        break;
    }

    if (state == jump_state::NONE && !double_jump)
    {
        if (m_pge->GetKey(olc::UP).bPressed && (GetTickCount() - double_jump_timer) < 250)
        {
            double_jump = true;
            jump_pos = position;
            state = jump_state::JUMPING;
        }
    }

    // run constant gravity
    if (state != jump_state::JUMPING)
    {
        old_position.y = position.y;
        position.y += GRAVITY * m_pge->GetElapsedTime();

        if (run_collision(false))
        {
            on_ground = true;
            double_jump = false;
            position.y = old_position.y;
        }
        else
            on_ground = false;
    }
}

void Player::movement()
{
    if ((m_pge->GetKey(olc::LEFT).bPressed || m_pge->GetKey(olc::LEFT).bHeld))
    {
        player_sprite.SetState("walking-left");
        last_animation_state = player_sprite.GetState();

        old_position.x = position.x;
        position.x -= SPEED * m_pge->GetElapsedTime();

        if (run_collision(false))
            position.x = old_position.x;
    }
    if ((m_pge->GetKey(olc::RIGHT).bPressed || m_pge->GetKey(olc::RIGHT).bHeld))
    {
        player_sprite.SetState("walking-right");
        last_animation_state = player_sprite.GetState();

        old_position.x = position.x;
        position.x += SPEED * m_pge->GetElapsedTime();

        if (run_collision(false))
            position.x = old_position.x;
    }
    if (m_pge->GetKey(olc::UP).bPressed)
    {
        if (state == jump_state::NONE && on_ground)
        {
            on_ground = false;
            jump_pos = position;
            state = jump_state::JUMPING;
        }
    }
    if (state != jump_state::JUMPING)
        state = jump_state::NONE;

    if (!(m_pge->GetKey(olc::RIGHT).bPressed || m_pge->GetKey(olc::RIGHT).bHeld) &&
        !(m_pge->GetKey(olc::LEFT).bPressed || m_pge->GetKey(olc::LEFT).bHeld))
    {
        if (last_animation_state == "walking-left")
            last_animation_state = "idle-left";
        else if (last_animation_state == "walking-right")
            last_animation_state = "idle-right";

        player_sprite.SetState(last_animation_state);
    }
}

bool Player::check_collision(const Map::tile& tile)
{
    return tile.position.x + tile.tile_size.x >= position.x + 3.5
        && tile.position.x < position.x + size.x + 3.5
        && tile.position.y + tile.tile_size.y >= position.y
        && tile.position.y < position.y + size.y;
}

void Player::interaction()
{
    // debug mode
    if (m_pge->GetKey(olc::F10).bReleased)
        debug_mode = !debug_mode;

    if (m_pge->GetKey(olc::DOWN).bReleased || m_pge->GetKey(olc::DOWN).bPressed)
        run_collision(true);  
}

bool Player::run_collision(bool interaction)
{
    for (auto& [name, tile] : *collidable_tiles)
    {
        if (tile->destroyed)
            continue;

        if (check_collision(*tile))
        {
            if ((name.compare("collectable") == 0 || name.compare("correct_key") == 0 || name.compare("next_level") == 0) && interaction)
                pickup_time = GetTickCount();
            if (name.compare("collectable") == 0 && key_state != key_state::HOLDING && interaction)
            {
                tile->destroyed = true;
                first_pickup = true;
                key_state = key_state::HOLDING;
                key_type = key_type::WRONG;
                return false;
            }
            else if (name.compare("correct_key") == 0 && key_state != key_state::HOLDING && interaction)
            {
                tile->destroyed = true;
                first_pickup = true;
                key_state = key_state::HOLDING;
                key_type = key_type::CORRECT;
                return false;
            }
            else if (name.compare("next_level") == 0 && key_state == key_state::HOLDING && interaction)
            {
                if (key_type == key_type::CORRECT)
                {
                    tile->destroyed = true;
                    key_state = key_state::NONE;
                    level++;
                }
                else
                    key_state = key_state::NONE;

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

    // Render collidables
    if (debug_mode)
    {
        for (auto& [name, tile] : *collidable_tiles)
        {
            if (tile->destroyed)
                continue;

            m_pge->DrawRect(tile->position, tile->tile_size, olc::RED);
        }
    }
}

void Player::update()
{
    movement();
    jump_movement();
    interaction();
    render();
}