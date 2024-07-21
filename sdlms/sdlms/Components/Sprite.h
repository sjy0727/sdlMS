#pragma once

#include "Component.h"
#include "wz/Property.hpp"
#include <SDL2/SDL.h>
#include <any>
#include <map>
#include <string>
#include <vector>

class Sprite : public Component
{
public:
     Sprite(SDL_Texture* texture, int w, int h);
     Sprite(wz::Node* node, int width, int height, uint8_t type);
     Sprite(wz::Node* node, int alpha = 255);
    ~Sprite() override;

    enum Type : uint8_t
    {
        NameTag,
        ChatBallon,
    };

private:
    Sprite() = default;

public:
    const auto   get_width() const { return width; }
    const auto   get_height() const { return height; }
    SDL_Texture* get_texture() const { return texture; }

    const auto get_delay() const { return delay; }
    const auto get_a0() const { return a0; }
    const auto get_a1() const { return a1; }
    const auto get_origin() const { return origin; }

    const auto get_z() const { return z; }

public:
    SDL_Texture* texture = nullptr;
    int          width   = 0;
    int          height  = 0;

    int       delay  = 0;
    int       a0     = 255;
    int       a1     = 255;
    SDL_Point origin = {0, 0};

    std::any z = 0;
};