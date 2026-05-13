#pragma once

#include "glm/ext/vector_float3.hpp"
#include <cassert>
#include <ECS/Scene.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <dlfcn.h>

#define CODE_ID 0
#define TRANSFORM_ID 1

using json = nlohmann::json;

namespace Sauce
{
    const std::string ENTITY_DLL_PATH = ".";
    struct Transform
    {
        Transform()
        {
            Position = {0.0f, 0.0f, 0.0f};
            Scale = {1.0f, 1.0f, 1.0f};
            Rotation = {0.0f, 0.0f, 0.0f};
        }
        glm::vec3 Position;
        glm::vec3 Scale;
        glm::vec3 Rotation;

        void print()
        {
            std::cout << "transform component\n";
        }
    };

    struct Code
    {
        void (*Start)();
        void (*Update)(float deltaTime);
        void (*OnDestroy)();
    };
}
