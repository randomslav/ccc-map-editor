#pragma once
#include "state.h"
#include "../camera.h"
#include "../map.h"

class EditState : public State {
    Camera camera;
    TileMap city_map;

    olc::GFX3D::PipeLine render_pipe;
    std::unique_ptr<olc::Sprite> selected;

    olc::GFX3D::vec3d mouse_target;
    olc::GFX3D::mat4x4 projection, proj_inv;

    float fov = 27.0f;

public:
    EditState(MapEditor &app);

    void update(float elapsed_time) override;
};
