#pragma once
#include "Core/World.h"
#include "Entity.h"
#include "wz/Property.hpp"

class FootHold : public Entity
{
public:
     FootHold(wz::Node* node, int page, int zmass, World* world);
    ~FootHold();

public:
    constexpr auto get_page() { return page; }

public:
    // fh的附加属性
    int page;
    int zmass;
    int prev;
    int next;
};
