#pragma once
#include "core.h"
#include <deque>
#include <stdlib.h>

#define TILE_SIZE 32

class Map
{
public:
    struct tile
    {
        olc::vi2d position;
        olc::vi2d tile_sheet_pos;
        olc::vi2d tile_size;
        bool destroyed;
    };

    olc::PixelGameEngine* m_pge;
    olc::vi2d map_size;
    olc::Renderable sprite_sheet;
    std::deque<Map::tile*> tiles;
    std::vector<std::pair<std::string, Map::tile*>> collidable_tiles;
public:

    // Load map from pyxel
    void loadMap(const std::string& path);

    void render();
};