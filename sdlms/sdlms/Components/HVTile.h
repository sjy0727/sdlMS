#pragma once
#include "Component.h"

// 平铺属性
class HVTile : public Component
{
public:
                   HVTile(int cx, int cy, bool htile, bool vtile) : cx(cx), cy(cy), htile(htile), vtile(vtile) {}
    constexpr auto get_cx() const { return cx; }
    constexpr auto get_cy() const { return cy; }

public:
    // 平铺间隔
    int cx;
    int cy;
    // 是否平铺
    bool htile = false;
    bool vtile = false;
};