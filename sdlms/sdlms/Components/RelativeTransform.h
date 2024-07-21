#pragma once

#include "Components/Component.h"
#include "Transform.h"

class RelativeTransform : public Component
{
public:
    RelativeTransform(Transform* tr) : position({0.0f, 0.0f}), rotation(0), flip(0), tr(tr) {}
    RelativeTransform(Transform* tr, SDL_FPoint p, int flip = 0) : position(p), rotation(0), flip(flip), tr(tr) {}
    RelativeTransform(Transform* tr, float x, float y, int flip = 0) : position({x, y}), rotation(0), flip(flip), tr(tr)
    {}
    ~RelativeTransform() override = default;

    SDL_FPoint get_position() const { return position; }
    float      get_rotation() const { return rotation; }
    int        get_flip() const { return flip; }
    auto       get_tr() { return tr; }

    void set_position(const SDL_FPoint& value) { position = value; }
    void set_rotation(const float& value) { rotation = value; }
    void set_flip(const int value) { flip = value; }
    void set_x(const float value) { position.x = value; }
    void set_y(const float value) { position.y = value; }

private:
    SDL_FPoint position;
    float      rotation;
    int        flip;
    Transform* tr = nullptr;
};
