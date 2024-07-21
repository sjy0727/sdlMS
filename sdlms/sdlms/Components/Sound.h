#pragma once
#include "Component.h"
#include "wz/Property.hpp"

class Sound : public Component
{
public:
    static Sound*                             load(wz::Node* node);
    static inline std::map<wz::Node*, Sound*> sound_map;

private:
    Sound() = default;
    Sound(wz::Node* node);

public:
    constexpr auto get_pcm_data() const { return &pcm_data; }
    constexpr auto get_freq() const { return freq; }
    constexpr auto get_offset() const { return offset; }
    constexpr auto get_delay() const { return delay; }
    constexpr auto get_play() const { return play; }

    void set_offset(unsigned int val) { offset = val; }
    void set_play(bool val) { play = val; }

public:
    std::vector<uint8_t> pcm_data;
    unsigned int         freq;
    unsigned int         offset;         // 记录当前播放的位置
    unsigned int         delay;          // 播放声音后下次播放的间隔,若为-1,则表示播放1次
    bool play = false;   // 当前是否播放
};
