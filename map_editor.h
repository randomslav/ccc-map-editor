#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics3D.h"
#include <map>
#include <memory>
#include <utility>
#include <stack>

#include "resource/resource_manager.h"
#include "states/edit.h"

class MapEditor : public olc::PixelGameEngine {
    std::stack<std::unique_ptr<State>> states;
    ResourceManager resource_manager;
public:
    MapEditor(){
        sAppName = "CCC Map Editor";
    }
    bool OnUserCreate() override {
        olc::GFX3D::ConfigureDisplay();
        states.push(std::make_unique<EditState>(*this));

        resource_manager.init();
        resource_manager.load_roads("roads.png");

        return true;
    }
    bool OnUserUpdate(float elapsed_time) override {
        auto &current_state = states.top();
        current_state->update(elapsed_time);

        auto new_state = current_state->extract_next();
        if(current_state->done()) states.pop();
        if(new_state != nullptr)  states.push(std::move(new_state));

        return !states.empty();
    }

    ResourceManager &resources(){ return resource_manager; }
};
