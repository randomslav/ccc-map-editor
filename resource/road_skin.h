#pragma once
#include "../olcPixelGameEngine.h"
#include <array>
#include <memory>
#include <string>

class RoadSkin {
    std::array<std::unique_ptr<olc::Sprite>, 16> sprites;
public:
    void load(std::string filename);

    olc::Sprite *get(bool left, bool right, bool top, bool bottom){
        int n = left * 8 + right * 4 + top * 2 + bottom;

        return sprites[n].get();
    }
};
