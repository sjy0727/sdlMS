#pragma once
#include "Component.h"
#include <cmath>
#include <optional>
// 线
class Line : public Component
{
public:
                         Line(SDL_FPoint m, SDL_FPoint n);
    constexpr auto       get_m() { return m; }                             // 获取线段起点
    constexpr auto       get_n() { return n; }                             // 获取线段终点
    auto                 get_min_x() const { return std::fmin(m.x, n.x); } // 获取线段最小x坐标
    auto                 get_max_x() const { return std::fmax(m.x, n.x); } // 获取线段最大x坐标
    auto                 get_min_y() const { return std::fmin(m.y, n.y); } // 获取线段最小y坐标
    auto                 get_max_y() const { return std::fmax(m.y, n.y); } // 获取线段最大y坐标
    constexpr auto       get_k() { return k; }                             // 获取斜率
    std::optional<float> get_y(float x);                                   // 根据x获取y
    std::optional<float> get_x(float y);                                   // 根据y获取x

private:
    SDL_FPoint           m;                        // 线段起点
    SDL_FPoint           n;                        // 线段终点
    std::optional<float> k         = std::nullopt; // 斜率,若k值不存在,可以判断是否是墙面
    std::optional<float> intercept = std::nullopt; // 截距
};