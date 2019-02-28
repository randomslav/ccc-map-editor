#include "edit.h"
#include "../map_editor.h"

EditState::EditState(MapEditor &app): State(app) {
    render_pipe.SetProjection(
        fov,
        (float)app.ScreenHeight()/app.ScreenWidth(),
        0.1f, 1000.0f,
        0.0, 0.0, app.ScreenWidth(), app.ScreenHeight()
    );

    city_map.load("example1.city");

    selected = std::make_unique<olc::Sprite>(1, 1);
    selected->GetData()[0] = olc::RED;

    projection = olc::GFX3D::Math::Mat_MakeProjection(fov, (float)app.ScreenHeight() / app.ScreenWidth(), 0.1f, 1000.0f);
    proj_inv = olc::GFX3D::Math::Mat_Inverse(projection);
}

void EditState::update(float elapsed_time){
    if(app.GetKey(olc::Key::W).bHeld) camera.position.y -= 2.0f * elapsed_time;
    if(app.GetKey(olc::Key::S).bHeld) camera.position.y += 2.0f * elapsed_time;
    if(app.GetKey(olc::Key::A).bHeld) camera.position.x -= 2.0f * elapsed_time;
    if(app.GetKey(olc::Key::D).bHeld) camera.position.x += 2.0f * elapsed_time;

    if(app.GetKey(olc::Key::Z).bHeld) camera.position.z += 5.0f * elapsed_time;
    if(app.GetKey(olc::Key::X).bHeld) camera.position.z -= 5.0f * elapsed_time;

    if(app.GetKey(olc::Key::Y).bHeld){
        fov += 5 * elapsed_time;
        std::cout << "fov " << fov << '\n';
        render_pipe.SetProjection(
            fov,
            (float)app.ScreenHeight()/app.ScreenWidth(),
            0.1f, 1000.0f,
            0.0, 0.0, app.ScreenWidth(), app.ScreenHeight()
        );
        projection = olc::GFX3D::Math::Mat_MakeProjection(fov, (float)app.ScreenHeight() / app.ScreenWidth(), 0.1f, 1000.0f);
        proj_inv = olc::GFX3D::Math::Mat_Inverse(projection);
    }
    if(app.GetKey(olc::Key::H).bHeld){
        fov -= 5 * elapsed_time;
        std::cout << "fov " << fov << '\n';
        render_pipe.SetProjection(
            fov,
            (float)app.ScreenHeight()/app.ScreenWidth(),
            0.1f, 1000.0f,
            0.0, 0.0, app.ScreenWidth(), app.ScreenHeight()
        );
        projection = olc::GFX3D::Math::Mat_MakeProjection(fov, (float)app.ScreenHeight() / app.ScreenWidth(), 0.1f, 1000.0f);
        proj_inv = olc::GFX3D::Math::Mat_Inverse(projection);
    }


    if(!app.GetKey(olc::Key::CTRL).bHeld) camera.position.z += -app.GetMouseWheel() / 120;

    if(app.GetKey(olc::Key::F).bHeld){
        auto rotation = olc::GFX3D::Math::Mat_MakeRotationX(1.0f * elapsed_time);
        camera.direction = olc::GFX3D::Math::Mat_MultiplyVector(rotation, camera.direction);
        camera.up        = olc::GFX3D::Math::Mat_MultiplyVector(rotation, camera.up);
    }
    if(app.GetKey(olc::Key::G).bHeld){
        auto rotation = olc::GFX3D::Math::Mat_MakeRotationX(-1.0f * elapsed_time);
        camera.direction = olc::GFX3D::Math::Mat_MultiplyVector(rotation, camera.direction);
        camera.up        = olc::GFX3D::Math::Mat_MultiplyVector(rotation, camera.up);
    }

    auto screen_to_world = [&](int x, int y){
        olc::GFX3D::vec3d mouse_dir = {
            2.0f * ((x / (float)app.ScreenWidth())  - 0.5f) / projection.m[0][0],
            2.0f * ((y / (float)app.ScreenHeight()) - 0.5f) / projection.m[1][1],
            1.0f,
            0.0f
        };

        auto look_target = olc::GFX3D::Math::Vec_Add(camera.position, camera.direction);
        auto mouse_view = olc::GFX3D::Math::Mat_PointAt(camera.position, look_target, camera.up);

        olc::GFX3D::vec3d  mouse_origin {0.0f, 0.0f, 0.0f};
        mouse_origin = olc::GFX3D::Math::Mat_MultiplyVector(mouse_view, mouse_origin);

        mouse_dir = olc::GFX3D::Math::Mat_MultiplyVector(mouse_view, mouse_dir);
        mouse_dir = olc::GFX3D::Math::Vec_Mul(mouse_dir, 1000.0f);
        mouse_dir = olc::GFX3D::Math::Vec_Add(mouse_origin, mouse_dir);

        olc::GFX3D::vec3d plane_p {0.0f, 0.0f, 0.0f};
        olc::GFX3D::vec3d plane_n {0.0f, 0.0f, 1.0f};

        float t;
        return olc::GFX3D::Math::Vec_IntersectPlane(plane_p, plane_n, mouse_origin, mouse_dir, t);
    };

    auto mouse_world = screen_to_world(app.GetMouseX(), app.GetMouseY());

    // Camera dragging
    if(app.GetMouse(2).bPressed)
        mouse_target = mouse_world;
    if(app.GetMouse(2).bHeld){
        camera.position.x -= mouse_world.x - mouse_target.x;
        camera.position.y -= mouse_world.y - mouse_target.y;
        mouse_world = mouse_target;
    }

    // Handle mouse clicks
    if(app.GetMouse(0).bHeld)
        city_map.set(std::floor(mouse_world.x), std::floor(mouse_world.y), true);
    if(app.GetMouse(1).bHeld)
        city_map.set(std::floor(mouse_world.x), std::floor(mouse_world.y), false);

    if(app.GetKey(olc::Key::CTRL).bHeld)
        city_map.at(std::floor(mouse_world.x), std::floor(mouse_world.y)).height += app.GetMouseWheel() / 120;

    // Put the camera where it belongs
    auto look_target = olc::GFX3D::Math::Vec_Add(camera.position, camera.direction);
    render_pipe.SetCamera(camera.position, look_target, camera.up);

    app.Clear(olc::BLACK);
    olc::GFX3D::ClearDepth();

    // Draw quads
    for(const auto &tile : city_map.all_tiles()){
        const TileMap::Tile &cell = tile.second;
        auto pos = olc::GFX3D::Math::Mat_MakeTranslation(cell.x, cell.y, 0.0f);
        if(cell.road){ // road
            olc::Sprite *texture = app.resources().ground();
            if(cell.road)
                texture = app.resources().road(
                    city_map.is_road(cell.x, cell.y - 1),
                    city_map.is_road(cell.x, cell.y + 1),
                    city_map.is_road(cell.x - 1, cell.y),
                    city_map.is_road(cell.x + 1, cell.y)
                );
            render_pipe.SetTransform(pos);
            render_pipe.SetTexture(texture);
            render_pipe.Render(app.resources().unity_quad().tris);
        } else {
            if(cell.height <= 0){ // ground or water
                olc::Sprite *texture = cell.height == 0 ? app.resources().ground() : app.resources().water();
                render_pipe.SetTransform(pos);
                render_pipe.SetTexture(texture);
                render_pipe.Render(app.resources().unity_quad().tris);
            } else if(cell.height > 0){ // building
                auto &walls = app.resources().building_mesh();

                // draw ground floor
                {
                    //auto pos = olc::GFX3D::Math::Mat_MakeTranslation(cell.x, cell.y, -0.2f);
                    pos.m[3][2] = -0.2f;
                    render_pipe.SetTexture(app.resources().door());
                    render_pipe.SetTransform(pos);
                    render_pipe.Render(walls.tris);
                }

                // draw walls
                render_pipe.SetTexture(app.resources().walls());
                for(int level = 1; level < cell.height; level++){
                    //auto pos = olc::GFX3D::Math::Mat_MakeTranslation(cell.x, cell.y, (level+1) * -0.2);
                    pos.m[3][2] = (level+1) * -0.2f;
                    render_pipe.SetTransform(pos);
                    render_pipe.Render(walls.tris);
                }

                // draw roof
                //auto pos = olc::GFX3D::Math::Mat_MakeTranslation(cell.x, cell.y, cell.height * -0.2);
                pos.m[3][2] = cell.height * -0.2;
                render_pipe.SetTransform(pos);
                render_pipe.SetTexture(app.resources().roof());
                render_pipe.Render(app.resources().unity_quad().tris);
            }
        }
    }

    // Highlight "selected" tile
    {
        olc::GFX3D::ClearDepth();
        auto transform = olc::GFX3D::Math::Mat_MakeTranslation(std::floor(mouse_world.x), std::floor(mouse_world.y), 0.0f);
        render_pipe.SetTransform(transform);
        render_pipe.SetTexture(selected.get());
        render_pipe.Render(app.resources().unity_quad().tris);
    }
}
