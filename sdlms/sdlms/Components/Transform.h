#pragma once

#include "Components/Component.h"

class Transform : public Component
{
public:
    Transform() : position({0, 0}), rotation(0), flip(0) {}
    Transform(SDL_FPoint p, int flip = 0, bool camera = false) : position(p), rotation(0), flip(0) {}
    Transform(float x, float y, int flip = 0, bool camera = false) :
        position({x, y}), rotation(0), flip(flip), camera(camera)
    {}
    ~Transform() override = default;

    SDL_FPoint get_position() const { return position; }
    float      get_rotation() const { return rotation; }
    int        get_flip() const { return flip; }
    auto       get_camera() const { return camera; }

    void set_position(const SDL_FPoint& value) { position = value; }
    void set_rotation(const float& value) { rotation = value; }
    void set_flip(const int& value) { flip = value; }
    void set_x(const float& value) { position.x = value; }
    void set_y(const float& value) { position.y = value; }

private:
    SDL_FPoint position = {0, 0};
    float      rotation = 0;
    int        flip     = 0;
    bool       camera   = false;
};
