#pragma once

#include <typeindex>
#include <typeinfo>
#include <unordered_map>

#include "Core/World.h"
#include "Entities/Entity.h"

class Entity;

// 组件类
class Component
{
public:
             Component() = default; // 默认构造函数
    virtual ~Component() = default; // 默认析构函数
    int      id          = 0;       // 组件ID

protected:
    Entity* owner = nullptr; // 组件所属实体

public:
    // 获取组件ID
    int get_id() const { return id; }

    // 设置组件ID
    void set_id(int value) { id = value; }

    // 获取组件所属实体
    Entity* get_owner() { return owner; }
    // 设置组件所属实体
    void set_owner(Entity* value) { owner = value; }
    // 获取组件所属实体的指定类型组件
    template<typename C>
    C* get_owner()
    {
        return dynamic_cast<C*>(owner);
    }

    // 获取组件所属实体的指定类型组件
    template<typename C>
    C* get_owner_component()
    {
        return static_cast<C*>(owner->get_component<C>());
    }
};