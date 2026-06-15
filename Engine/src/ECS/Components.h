#pragma once

#include "Render/Mesh.h"
#include "ScriptableEntity.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <functional>
#include <string>
#include <dlfcn.h>

#define CODE_ID 0
#define TRANSFORM_ID 1

namespace Sauce
{
    #define SCRIPT_CIDX 0
    #define TRANSFORM_CIDX 1
    #define MESH_INSTANCE_CIDX 2
    #define CAMERA_CIDX 3

    struct TransformComponent
    {
        glm::vec3 Translation;
        glm::vec3 Rotation;
        glm::vec3 Scale;

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;

        operator glm::mat4 ()
        {
            glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
        }
    };

    struct Script
    {
        std::shared_ptr<ScriptableEntity> Instance = nullptr;
        std::string ScriptName;

        std::function<std::shared_ptr<ScriptableEntity>()> InitializeScript;
        std::function<void(ScriptableEntity*)> DestroyScript;
    };

    struct CameraComponent
    {
        glm::mat4 View;
        bool IsMain;
        operator const glm::mat4 &() { return View; }
    };

    struct MeshInstance
    {
        Mesh *PMesh = nullptr;
        bool IsVisible;
    };
}
