#pragma once

#include "glm/ext/vector_float3.hpp"
#include <iostream>
namespace Sauce
{
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
}
