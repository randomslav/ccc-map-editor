#pragma once
#include "../olcPixelGameEngine.h"
#include <memory>

#include "road_skin.h"

class ResourceManager {
    RoadSkin road_sprites;

    std::unique_ptr<olc::Sprite> roof_sprite, walls_sprite;

    olc::GFX3D::mesh quad, walls_mesh;
    std::unique_ptr<olc::Sprite> ground_sprite, water_sprite, door_sprite;
public:
    void init(){

        water_sprite = std::make_unique<olc::Sprite>(1, 1);
        water_sprite->SetPixel(0, 0, olc::BLUE);

        quad.tris = {
            { 0.0f, 0.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f, 1.0f,   1.0f, 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,     1.0f, 1.0f, 0.0f, },
            { 0.0f, 0.0f, 0.0f, 1.0f,   1.0f, 1.0f, 0.0f, 1.0f,   1.0f, 0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,     1.0f, 0.0f, 0.0f, },
        };

        walls_mesh.tris =
        {
            // EAST
            { 1.0f, 0.0f, 0.0f, 1.0f,        1.0f, 1.0f, 0.0f, 1.0f,        1.0f, 1.0f, 0.2f, 1.0f,        1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f,         0.0f, 0.0f, 0.0f, },
            { 1.0f, 0.0f, 0.0f, 1.0f,        1.0f, 1.0f, 0.2f, 1.0f,        1.0f, 0.0f, 0.2f, 1.0f,        1.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f,         0.0f, 1.0f, 0.0f, },

            // WEST
            { 0.0f, 0.0f, 0.2f, 1.0f,        0.0f, 1.0f, 0.2f, 1.0f,        0.0f, 1.0f, 0.0f, 1.0f,        0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f,         1.0f, 0.0f, 0.0f, },
            { 0.0f, 0.0f, 0.2f, 1.0f,        0.0f, 1.0f, 0.0f, 1.0f,        0.0f, 0.0f, 0.0f, 1.0f,        0.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f,         1.0f, 1.0f, 0.0f, },

            // TOP
            { 0.0f, 1.0f, 0.0f, 1.0f,        0.0f, 1.0f, 0.2f, 1.0f,        1.0f, 1.0f, 0.2f, 1.0f,        1.0f, 0.0f, 0.0f,    0.0f, 0.0f, 0.0f,         0.0f, 1.0f, 0.0f,   },
            { 0.0f, 1.0f, 0.0f, 1.0f,        1.0f, 1.0f, 0.2f, 1.0f,        1.0f, 1.0f, 0.0f, 1.0f,        1.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,         1.0f, 1.0f, 0.0f,   },

            // BOTTOM
            { 1.0f, 0.0f, 0.2f, 1.0f,        0.0f, 0.0f, 0.2f, 1.0f,        0.0f, 0.0f, 0.0f, 1.0f,        0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f,         1.0f, 0.0f, 0.0f, },
            { 1.0f, 0.0f, 0.2f, 1.0f,        0.0f, 0.0f, 0.0f, 1.0f,        1.0f, 0.0f, 0.0f, 1.0f,        0.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f,         1.0f, 1.0f, 0.0f, },
        };

        road_sprites.load("roads.png");
        roof_sprite  = std::make_unique<olc::Sprite>("roof.png");
        walls_sprite = std::make_unique<olc::Sprite>("walls.png");
        door_sprite  = std::make_unique<olc::Sprite>("door.png");
        ground_sprite = std::make_unique<olc::Sprite>("ground.png");
    }

    void load_roads(std::string filename){
        road_sprites.load(filename);
    }

    // ************ GET RESOURCES ************

    olc::Sprite *road(bool left, bool right, bool top, bool bottom){
        return road_sprites.get(left, right, top, bottom);
    }

    olc::Sprite *ground(){
        return ground_sprite.get();
    }

    olc::Sprite *roof(){
        return roof_sprite.get();
    }

    olc::Sprite *door(){
        return door_sprite.get();
    }

    olc::Sprite *walls(){
        return walls_sprite.get();
    }

    olc::Sprite *water(){
        return water_sprite.get();
    }

    olc::GFX3D::mesh &unity_quad(){
        return quad;
    }

    olc::GFX3D::mesh &building_mesh(){
        return walls_mesh;
    }
};
