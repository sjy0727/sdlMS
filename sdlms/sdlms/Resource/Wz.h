#pragma once

#include "Resource.h"
#include <wz/File.hpp>

class Wz final : public Resource
{
public:
     Wz() = delete;
     Wz(const std::string& filename_prefix);

    wz::File* Base;
    wz::File* Character;
    wz::File* Effect;
    wz::File* Etc;
    wz::File* Item;
    wz::File* List;
    wz::File* Map;
    wz::File* Mob;
    wz::File* Morph;
    wz::File* Npc;
    wz::File* Quest;
    wz::File* Reactor;
    wz::File* Skill;
    wz::File* Sound;
    wz::File* String;
    wz::File* TamingMob;
    wz::File* UI;
};
