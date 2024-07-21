#pragma once
#include "Component.h"
#include "Transform.h"
#include <optional>

// 限制Transform的移动范围
class LimitTransform : public Component
{
public:
    LimitTransform(Transform* tr, std::optional<SDL_FPoint> h, std::optional<SDL_FPoint> v) : h(h), v(v), tr(tr) {}
    auto& get_h() { return h; }
    auto& get_v() { return v; }
    auto& get_tr() { return tr; }

private:
    std::optional<SDL_FPoint> h  = std::nullopt;
    std::optional<SDL_FPoint> v  = std::nullopt;
    Transform*                tr = nullptr;
};