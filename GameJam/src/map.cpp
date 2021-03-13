#include "headers/map.h"
#include "headers/json.hpp"

using json = nlohmann::json;

void Map::loadMap(const std::string& path)
{
    sprite_sheet.Load("./sprites/tilesheet.png");
    std::ifstream i(path);
    json j = json::parse(i);

    map_size.y = j.at("tileshigh");
    map_size.x = j.at("tileswide");
    olc::vi2d tile_size = { j.at("tilewidth"), j.at("tileheight") };

    int tile_sheet_width = sprite_sheet.Sprite()->width / tile_size.x;

    auto layers = j.at("layers");

    // Loop all the layers that we grab from the json file
    for (auto& layer : layers)
    {
        // Loop each tile in the layer
        for (auto& tile : layer.at("tiles"))
        {
            // This is the tile ID (position in the tilesheet / spritesheet) determined by Pyxel
            int tile_id = tile.at("tile");
            // If tileID is -1 that means the tile has no tile aka is transparent
            if (tile_id != -1)
            {
                std::string layer_name = layer.at("name");
                int x = tile.at("x");
                int y = tile.at("y");
                x *= tile_size.x;
                y *= tile_size.y;

                int tile_sheet_pos_y = 0;
                int tile_sheet_pos_x = 0;

                // This is an easy way to know what Row in the tilesheet we want to grab the tile from
                if (tile_id > tile_sheet_width)
                {
                    int i = tile_id;
                    while (i > tile_sheet_width)
                    {
                        tile_sheet_pos_y += 1;
                        i -= tile_sheet_width;
                    }
                    tile_sheet_pos_x = i - 1;
                }
                else
                {
                    tile_sheet_pos_x = tile_id - 1;
                }

                // Make sure we grab just 1 tile / sprite
                tile_sheet_pos_y *= tile_size.y;
                tile_sheet_pos_x *= tile_size.x;

                // Here we store all tile values into a vector of tiles
                // The data we store is basically the position on the map where it's drawn
                // and also the position on the tileSheet where it's stored
                tiles.push_back(new Map::tile{ { x, y }, { tile_sheet_pos_x, tile_sheet_pos_y }, {TILE_SIZE, TILE_SIZE}, false });

                // Used for my own collisions, ignore this (Credits to Witty bits for the collision struct from the relay race)
                if (layer_name == "Colliders")
                    collidable_tiles.push_back(std::make_pair("map_terrain", tiles.back()));
                if (layer_name == "Collectables")
                    collidable_tiles.push_back(std::make_pair("collectable", tiles.back()));
                
            }
        }
    }
}

void Map::render()
{
    for (auto* tile : tiles)
    {
        if (tile->destroyed)
            continue;

        m_pge->DrawPartialDecal(tile->position, sprite_sheet.Decal(), tile->tile_sheet_pos, { TILE_SIZE, TILE_SIZE });
    }
}