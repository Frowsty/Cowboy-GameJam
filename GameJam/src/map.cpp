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
    olc::vi2d tileSize = { j.at("tilewidth"), j.at("tileheight") };

    int tileSheetWidth = sprite_sheet.Sprite()->width / tileSize.x;

    auto layers = j.at("layers");

    // Loop all the layers that we grab from the json file
    for (auto& layer : layers)
    {
        // Loop each tile in the layer
        for (auto& tile : layer.at("tiles"))
        {
            // This is the tile ID (position in the tilesheet / spritesheet) determined by Pyxel
            int tileID = tile.at("tile");
            // If tileID is -1 that means the tile has no tile aka is transparent
            if (tileID != -1)
            {
                std::string layerName = layer.at("name");
                int x = tile.at("x");
                int y = tile.at("y");
                x *= tileSize.x;
                y *= tileSize.y;

                int tileSheetPosY = 0;
                int tileSheetPosX = 0;

                // This is an easy way to know what Row in the tilesheet we want to grab the tile from
                if (tileID > tileSheetWidth)
                {
                    int i = tileID;
                    while (i > tileSheetWidth)
                    {
                        tileSheetPosY += 1;
                        i -= tileSheetWidth;
                    }
                    tileSheetPosX = i - 1;
                }
                else
                {
                    tileSheetPosX = tileID - 1;
                }

                // Make sure we grab just 1 tile / sprite
                tileSheetPosY *= tileSize.y;
                tileSheetPosX *= tileSize.x;

                // Here we store all tile values into a vector of tiles
                // The data we store is basically the position on the map where it's drawn
                // and also the position on the tileSheet where it's stored
                tiles.push_back(new Map::tile{ { x, y }, { tileSheetPosX, tileSheetPosY }, {TILE_SIZE, TILE_SIZE}, false });

                // Used for my own collisions, ignore this (Credits to Witty bits for the collision struct from the relay race)
                if (layerName == "Colliders")
                    collidable_tiles.push_back(std::make_pair("map_terrain", tiles.back()));
                if (layerName == "Collectables")
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

        m_pge->DrawPartialDecal(tile->position, sprite_sheet.Decal(), tile->tile_sheet_pos, { 32, 32 });
    }
}