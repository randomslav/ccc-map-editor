#pragma once
#include <memory>

class MapEditor;

class State {
protected:
    MapEditor &app;
    bool done_flag = false;
    std::unique_ptr<State> next_state;
public:
    State(MapEditor &app): app(app) {}
    virtual ~State(){}

    bool done(){ return done_flag; }
    std::unique_ptr<State> extract_next(){ return std::move(next_state); }

    virtual void update(float elapsed_time) = 0;
};
