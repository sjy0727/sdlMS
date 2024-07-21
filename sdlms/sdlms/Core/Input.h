#pragma once

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_joystick.h>
#include <SDL2/SDL_keyboard.h>

#include <unordered_set>

// 定义输入类
class Input
{
public:
    // 定义鼠标按键枚举
    enum class MouseButton
    {
        ButtonLeft   = 1, // 左键
        ButtonMiddle = 2, // 中键
        ButtonRight  = 3  // 右键
    };

private:
    // 定义静态变量，存储按下的键、按下的键（瞬时的）、释放的键（瞬时的）
    inline static std::unordered_set<SDL_Keycode> pressed_keys            = {};
    inline static std::unordered_set<SDL_Keycode> pressed_keys_momentary  = {};
    inline static std::unordered_set<SDL_Keycode> released_keys_momentary = {};

    // 定义静态变量，存储按下的鼠标按钮、按下的鼠标按钮（瞬时的）、释放的鼠标按钮（瞬时的）
    inline static std::unordered_set<MouseButton> pressed_mouse            = {};
    inline static std::unordered_set<MouseButton> pressed_mouse_momentary  = {};
    inline static std::unordered_set<MouseButton> released_mouse_momentary = {};

public:
    // 定义事件类型枚举
    enum class EventType
    {
        KeyPressed,   // 按下键
        KeyReleased,  // 释放键
        MousePressed, // 按下鼠标按钮
        MouseReleased // 释放鼠标按钮
    };

    // 定义静态函数，判断键是否按下
    static bool is_key_pressed(SDL_Keycode key);
    // 定义静态函数，判断键是否被按住
    static bool is_key_held(SDL_Keycode key);
    // 定义静态函数，判断键是否释放
    static bool is_key_released(SDL_Keycode key);

    // 定义静态函数，判断鼠标按钮是否按下
    static bool is_mouse_button_pressed(MouseButton button);
    // 定义静态函数，判断鼠标按钮是否被按住
    static bool is_mouse_button_held(MouseButton button);
    // 定义静态函数，判断鼠标按钮是否释放
    static bool is_mouse_button_released(MouseButton button);

    // 定义静态函数，处理输入事件
    static void process_input_event(EventType type, const SDL_Event& event);

    // 定义静态函数，更新瞬时的键
    static void update_momentary_keys();
};