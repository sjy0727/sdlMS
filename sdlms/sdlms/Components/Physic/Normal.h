#pragma once
#include "../Component.h"
#include <optional>

// 正常物理状态,非水下,飞翔
class Normal : public Component
{
public:
    Normal() = default;

public:
    float                     hspeed       = 0.0;                           // 水平速度
    std::optional<SDL_FPoint> hspeed_limit = SDL_FPoint {-125.0f, 125.0f};  // 水平速度限制
    float                     vspeed       = 0.0;                           // 垂直速度
    std::optional<SDL_FPoint> vspeed_limit = SDL_FPoint {-5000.0f, 670.0f}; // 垂直速度限制
    float                     hforce       = 0.0;                           // 水平力
    float                     vforce       = 0.0;                           // 垂直力
    float                     hacc         = 0.0;                           // 水平加速度
    float                     vacc         = 0.0;                           // 垂直加速度
    enum : uint8_t                                                          // 状态
    {
        Ground,
        Air,
        Climb,
    };
    uint8_t type = Air;

    enum : uint8_t // 按键状态
    {
        None,
        Up,
        Left,
        Down,
        Right,
    };
    uint8_t vkey = None; // 上下按键
    uint8_t hkey = None; // 左右按键
    bool lalt = false; // 左Alt键是否被按下
    bool lctrl = false; // 左Ctrl键是否被按下
};