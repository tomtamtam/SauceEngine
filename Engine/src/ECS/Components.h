#pragma once

#include "ECS/Entity.h"
#include "glm/glm.hpp"
#include <functional>
#include <iostream>
#include <string>
#include <dlfcn.h>

#define CODE_ID 0
#define TRANSFORM_ID 1

namespace Sauce
{
    struct TransformComponent
    {
        glm::mat4 Transform;

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::mat4 &transform) : Transform(transform) {}

        operator const glm::mat4&() const { return Transform; }


    };

    struct Code
    {
        Code() {}
        Code(Entity e)
        {
            Start = [e]() mutable {e.Start();};
            Update = [e]() mutable {e.Update();};
            OnDestroy = [e]() mutable {e.OnDestroy();};
        }
        std::function<void()> Start;
        std::function<void()> Update;
        std::function<void()> OnDestroy;
    };
}
