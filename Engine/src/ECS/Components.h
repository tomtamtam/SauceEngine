#pragma once

#include "glm/ext/vector_float3.hpp"
namespace Sauce
{
    struct Transform
    {
        glm::vec3 Position;
        glm::vec3 Scale;
        glm::vec3 Rotation;
    };
}
