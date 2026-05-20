#pragma once

#include "ScriptableEntity.h"
#include "glm/ext/matrix_float4x4.hpp"
#include <functional>
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

    struct Script
    {
        std::shared_ptr<ScriptableEntity> Instance = nullptr;
        std::string ScriptName;

        std::function<std::shared_ptr<ScriptableEntity>()> InitializeScript;
        std::function<void(ScriptableEntity*)> DestroyScript;
    };
}
