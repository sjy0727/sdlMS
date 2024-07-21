#pragma once

#include "Components/Component.h"
#include <optional>
#include <tuple>
#include <variant>
#include <vector>

#include "AnimatedSprite.h"
#include "Sprite.h"

class DistanceSprite : public Component
{
public:
                    DistanceSprite()          = default;
    ~               DistanceSprite() override = default;
    constexpr auto& get_dissprs() { return dissprs; }
    constexpr auto& get_hs() { return hs; }
    constexpr auto& get_vs() { return vs; }
    void            add(std::optional<SDL_FPoint>                             h,
                        std::optional<SDL_FPoint>                             v,
                        std::optional<std::variant<Sprite*, AnimatedSprite*>> disspr)
    {
        hs.push_back(h);
        vs.push_back(v);
        dissprs.push_back(disspr);
    }

private:
    std::vector<std::optional<std::variant<Sprite*, AnimatedSprite*>>> dissprs;
    std::vector<std::optional<SDL_FPoint>>                             hs; // 水平范围
    std::vector<std::optional<SDL_FPoint>>                             vs; // 垂直范围
};
