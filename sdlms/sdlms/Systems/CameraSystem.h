#pragma once
#include "Components/Camera.h"
#include "Components/Transform.h"
#include "Systems/System.h"

class CameraSystem : public System
{
public:
    void run(World& world) override;

private:
    void update_camera(Camera* cam, Transform* tr, World& world);
};
