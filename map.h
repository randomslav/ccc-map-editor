#pragma once
#include <string>
#include <fstream>

class TileMap {
public:
    struct Tile {
        int height = 0;
        int x = 0, y = 0;
        bool road = false;
        bool building = true;
    };
private:
    std::map<std::pair<int, int>, Tile> tiles;
public:
    void load(std::string sFilename){
        std::ifstream file(sFilename, std::ios::in | std::ios::binary);
        int width, height;
        file.read((char*)&width, sizeof(int));
        file.read((char*)&height, sizeof(int));
        tiles.clear();
        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {
                Tile tile;
                file.read((char*)&tile, sizeof(Tile));

                tiles[{tile.x, tile.y}] = tile;
            }
        }
    }

    const auto &all_tiles(){ return tiles; }
    Tile &at(int x, int y){ return tiles[{x, y}]; }

    bool is_road(int x, int y){
        auto it = tiles.find({x, y});
        return it != tiles.end() && it->second.road;
    }

    void set(int x, int y, bool road){
        auto it = tiles.find({x, y});
        if(it == tiles.end()){
            tiles[{x, y}] = {0, x, y, road, false};
        } else it->second.road = road;
    }
};
