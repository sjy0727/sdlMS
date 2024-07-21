#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <vector>

#include "Sprite.h"
#include "wz/Property.hpp"

class AnimatedSprite : public Component
{
private:
    std::vector<Sprite*> sprites; // 用于存储动画中的所有帧
    int                  anim_index; // 用于存储当前动画的帧索引
    int                  anim_time; // 用于存储当前动画的播放时间
    int                  anim_size; // 用于存储动画中帧的数量
    bool                 animate = true;  // 是否播放动画,特殊情况,冰冻状态下会暂停动画
    bool                 z       = false; // ziazag用于表示是否使用锯齿形播放动画

public:
    explicit AnimatedSprite(wz::Node* node, int alpha = 255);
    ~        AnimatedSprite() override;
    void     add_anim_time(const int val) { anim_time += val; } // 用于增加动画的播放时间

    int                   get_anim_time() const { return anim_time; }
    int                   get_anim_index() const { return anim_index; }
    int                   get_anim_size() const { return anim_size; }
    bool                  get_zigzag() const { return z; }
    bool                  get_animate() const { return animate; }
    Sprite*               get_current_sprite() { return sprites[anim_index]; }
    int                   get_anim_delay() { return sprites[anim_index]->delay; }
    int                   get_anim_width() { return sprites[anim_index]->get_width(); }
    int                   get_anim_height() { return sprites[anim_index]->get_height(); }
    std::vector<Sprite*>& get_sprites() { return sprites; }

    void set_anim_index(const int val) { anim_index = val; }
    void set_anim_time(const int val) { anim_time = val; }
    void set_animate(const bool val) { animate = val; }
};