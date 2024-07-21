#pragma once
#include "Components/Sound.h"
#include "World.h"
#include "wz/Property.hpp"

// 用于加载地图中各种组件
class Map
{
public:
    static void     load(int map_id, World* world);                // 加载地图
    static void     load_obj(wz::Node* node, World* world);        // 加载地图中的对象
    static void     load_tile(wz::Node* node, World* world);       // 加载地图中的瓦片
    static void     load_background(wz::Node* node, World* world); // 加载地图中的背景
    static void     load_string(int map_id, World* world);         // 加载地图中的字符串
    static void     load_foothold(wz::Node* node, World* world);   // 加载地图中的踏板
    static void     load_life(wz::Node* node, World* world);       // 加载地图中的生命
    static void     load_border(wz::Node* node, World* world);     // 加载地图中的边界
    static void     load_ladderRope(wz::Node* node, World* world); // 加载地图中的梯子
    static void     load_portal(wz::Node* node, World* world);     // 加载地图中的传送门
    static void     load_bgm(wz::Node* node, World* world);        // 加载地图中的背景音乐
    static void     clean_up(World* world);                        // 清理地图
    static uint32_t get_map_id();                                  // 获取地图ID

private:
    static wz::Node*       load_map_node(int map_id, World* world);// 加载地图节点
    static void            set_map_id(uint32_t id);// 设置地图ID
    static inline uint32_t map_id = 0;// 静态变量，用于存储地图ID
};