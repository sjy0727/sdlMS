#pragma once

#include <SDL2/SDL.h>

// 计算线条方向向量
inline void CalculateLineDirection(int x1, int y1, int x2, int y2, float& dx, float& dy)
{
    float length = SDL_sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    dx           = (x2 - x1) / length;
    dy           = (y2 - y1) / length;
}

// 使用多边形绘制具有指定宽度的线条
inline void SDL_RenderDrawThickLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int thickness)
{
    if (thickness <= 1)
    {
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        return;
    }

    float dx, dy;
    CalculateLineDirection(x1, y1, x2, y2, dx, dy);

    float px = -dy * thickness / 2;
    float py = dx * thickness / 2;

    const auto& color = SDL_Color {0, 255, 0, 255};

    SDL_Vertex vertices[4] = {
        {SDL_FPoint {static_cast<float>(x1 + px), static_cast<float>(y1 + py)}, color, SDL_FPoint {0, 0}},
        {SDL_FPoint {static_cast<float>(x1 - px), static_cast<float>(y1 - py)}, color, SDL_FPoint {0, 0}},
        {SDL_FPoint {static_cast<float>(x2 - px), static_cast<float>(y2 - py)}, color, SDL_FPoint {0, 0}},
        {SDL_FPoint {static_cast<float>(x2 + px), static_cast<float>(y2 + py)}, color, SDL_FPoint {0, 0}}};

    int indices[6] = {0, 1, 2, 2, 3, 0};

    SDL_RenderGeometry(renderer, nullptr, vertices, 4, indices, 6);
}