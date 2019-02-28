#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics3D.h"

struct Camera {
    olc::GFX3D::vec3d position  {0.0f, 0.0f, -10.0f},
                      direction {0.0f, 0.0f,   1.0f},
                      up        {0.0f, 1.0f,   0.0f};
};
