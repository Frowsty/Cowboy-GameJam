#include "headers/map.h"
#include "headers/json.hpp"
#include <stdlib.h>

using json = nlohmann::json;

void Map::loadMap(const std::string& path)
{
    tiles.clear();
    collidable_tiles.clear();
    sprite_sheet.Load("./sprites/tilesheet.png");
    std::ifstream i(path);
    json j = json::parse(i);
    map_size.y = j.at("tileshigh");
    map_size.x = j.at("tileswide");
    olc::vi2d tile_size = { j.at("tilewidth"), j.at("tileheight") };
    olc::Sprite temp_sprite = olc::Sprite("./sprites/tilesheet.png");

    int tile_sheet_width = sprite_sheet.Sprite()->width / tile_size.x;
    auto layers = j.at("layers");

    srand(time(NULL));
    int random_key = rand() % 7 + 1;
    int key_id = 1;
    // Loop all the layers that we grab from the json file
    for (const auto& layer : layers)
    {
        // Loop each tile in the layer
        for (const auto& tile : layer.at("tiles"))
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

                // Here we store all tile values into a vector of tiles and determine the height of each collidable block
                // The data we store is basically the position on the map where it's drawn
                // and also the position on the tileSheet where it's stored
                if (layer_name != "collectables" && layer_name != "next_level")
                {
                    int tile_height = 0;
                    for (int i = tile_sheet_pos_y + 1; i < tile_sheet_pos_y + tile_size.y; i++) {
                        olc::Pixel pixel = temp_sprite.GetPixel(tile_sheet_pos_x - (tile_sheet_pos_x / tile_size.y) + (tile_size.x / 2), i);
                        if (std::abs(tile_sheet_pos_y - i) >= tile_size.y - 1)
                        {
                            tile_height = tile_size.y;
                            break;
                        }
                        if (pixel.a == 0) {
                            tile_height = std::abs(tile_sheet_pos_y - i);
                            break;
                        }
                    }
                    tiles.push_back(new Map::tile{ { x - TILE_SIZE, y }, { tile_sheet_pos_x, tile_sheet_pos_y }, { TILE_SIZE, tile_height }, false });
                }
                else
                    tiles.push_back(new Map::tile{ { x - TILE_SIZE, y }, { tile_sheet_pos_x, tile_sheet_pos_y }, { TILE_SIZE, TILE_SIZE }, false });

                // If the layer_name is related to anything collidable we shall add it to collidable tiles
                if (layer_name == "colliders")
                    collidable_tiles.push_back(std::make_pair("map_terrain", tiles.back()));

                if (layer_name == "obstacles")
                    collidable_tiles.push_back(std::make_pair("map_terrain", tiles.back()));

                if(layer_name == "next_level")
                    collidable_tiles.push_back(std::make_pair("next_level", tiles.back()));

                if (layer_name == "collectables")
                {
                    if (key_id == random_key)
                        collidable_tiles.push_back(std::make_pair("correct_key", tiles.back()));
                    else
                        collidable_tiles.push_back(std::make_pair("collectable", tiles.back()));

                    key_id++;
                }
            }
        }
    }
}

void Map::render()
{
    m_pge->Clear({ 62, 190, 237 });
    for (const auto* tile : tiles)
    {
        if (tile->destroyed)
            continue;

        m_pge->DrawPartialDecal(tile->position, sprite_sheet.Decal(), tile->tile_sheet_pos, { TILE_SIZE, TILE_SIZE });
    }
}