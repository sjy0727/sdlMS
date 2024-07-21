#include "Line.h"
#include <algorithm>

Line::Line(SDL_FPoint m, SDL_FPoint n) : m(m), n(n)
{
    auto [x1, y1] = m;
    auto [x2, y2] = n;
    if (x1 != x2)
    {
        k         = (y2 - y1) / (x2 - x1); // 斜率
        intercept = y1 - k.value() * x1;   // 截距
    }
}

std::optional<float> Line::get_y(float x)
{
    // 判断x值是否在线段的范围内，然后在k有值的情况下计算y值并返回
    [[likely]] if (x >= get_min_x() && x <= get_max_x())
    {
        [[likely]] if (k.has_value())
        {
            return k.value() * x + intercept.value();
        }
    }
    return std::nullopt;
}

std::optional<float> Line::get_x(float y)
{
    // 判断y值是否在线段的范围内，然后在k有值的情况下计算x值并返回
    [[likely]] if (y >= get_min_y() && y <= get_max_y())
    {
        [[likely]] if (k.has_value())
        {
            return (y - intercept.value()) / k.value();
        }
    }
    return std::nullopt;
}
