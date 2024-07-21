#pragma once
#include "Component.h"
#include "Core/MathHelper.h"

// 随机产生按键输出组件
class RandomInput : public Component
{
public:
         RandomInput(int val = 60) : count(MathHelper::Random<unsigned int>(0, val) + 100) {}
    auto get_count() const { return count; }
    auto get_tick() const { return tick; }
    void set_count(const unsigned int value) { count = value; }
    void set_tick(const unsigned int value) { tick = value; }

private:
    unsigned int count = 0;
    unsigned int tick = 0;
};