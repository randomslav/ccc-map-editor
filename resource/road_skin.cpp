#include "road_skin.h"
#include "../olcPGEX_ImageManip.h"

void RoadSkin::load(std::string filename){
    auto sprite_sheet = std::make_unique<olc::Sprite>(filename);

    for(int i = 0; i < 4; i++)
    for(int j = 0; j < 4; j++){
        sprites[i * 4 + j].reset(olc::ImageManip::Crop(sprite_sheet.get(), 96 * j, 96 * i, 96, 96));
    }
}
