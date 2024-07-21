#include "Components/Camera.h"
#include "Components/Physic/Normal.h"
#include "Components/Player.h"
#include "Components/Sound.h"
#include "Components/Video.h"
#include "Core/FreeType.h"
#include "Core/Map.h"
#include "Core/World.h"
#include "Entities/Character.h"
#include "Entities/Mob.h"
#include "Resource/Wz.h"
#include "Systems/CameraSystem.h"
#include "Systems/DeltaTimeSystem.h"
#include "Systems/InputSystem.h"
#include "Systems/PhysicSystem.h"
#include "Systems/RenderSystem.h"
#include "Systems/SoundSystem.h"
#include "Systems/SpriteSystem.h"
#include "Systems/TransformSystem.h"
#include <SDL2/SDL.h>
#include <cstdio>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

int width = 800;
int height = 600;

void main_loop()
{
    World *world = World::get_world();
    [[unlikely]]
    if (world->is_game_quit())
    {
#ifdef __EMSCRIPTEN__
        emscripten_cancel_main_loop(); /* this should "kill" the app. */
#else
        exit(0);
#endif
    }
    world->poll_events();
    world->tick_delta_time();

    Window::clear();
    world->process_systems();
    Window::update();
}

int main(int argc, char *argv[])
{
    World world;
#ifdef __ANDROID__
    Wz *wz = new Wz("/sdcard/Data/"); // wz文件路径
    FreeType::init("/sdcard/Data/"); // 字体文件路径
#else
    Wz *wz = new Wz("./Data/"); // wz文件路径
    FreeType::init("./Data/"); // 字体文件路径
#endif
    world.add_resource(wz);

    Window::create_window("sdlMS", width, height); // 创建窗口

    // 添加system到std::vector<System *> system_list
    world.add_system(new SoundSystem());

    world.add_system(new InputSystem());

    world.add_system(new PhysicSystem());

    world.add_system(new DeltaTimeSystem());

    world.add_system(new SpriteSystem());

    world.add_system(new TransformSystem());

    world.add_system(new CameraSystem());

    world.add_system(new RenderSystem());

    Camera *camera = new Camera(0, 0, width, height);
    Player *pla = new Player(); // 玩家控制的角色
    {
        Character *cha = new Character(&world, {0, 0});
        Normal *nor = new Normal();

        cha->add_component(camera); // 角色添加相机
        cha->add_component(nor); // 角色添加普通物理
        cha->add_component(pla); // 角色添加玩家控制

        world.add_component(camera, 0);
        world.add_component(nor);
        world.add_component(pla);
    }
    // {
    //     Mob *mob = new Mob(&world, {0, 0});
    //     Normal *nor = new Normal();
    //     mob->add_component(nor);
    //     mob->add_component(pla);
    //     mob->add_component(camera);
    //     world.add_component(camera, 0);
    //     world.add_component(nor);
    //     world.add_component(pla);
    // }

    Map::load(104040000, &world);
    // Map::load(105040300, &world);

    // Video *vid = Video::load("./tong.mp4", 244, 184);
    // if (vid != nullptr)
    // {
    //     Entity *ent = new Entity();
    //     Transform *t = new Transform{(float)244, (float)184};
    //     ent->add_component(vid);
    //     ent->add_component(t);
    //     world.add_component(t,123000);
    //     // world.add_component(t,9250042);
    //     world.add_component(vid);
    // }
    world.tick_delta_time();

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, 1);
#else
    while (true)
    {
        main_loop();
    }
#endif
    return 0;
}