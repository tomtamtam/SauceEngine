#pragma once

#include "Core/UUID.h"
#include "Render/Mesh.h"
#include "ScriptableEntity.h"
#include <cstdint>
#include <memory>
#include <functional>
#include <string>
#include <dlfcn.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Sauce
{
    #define SCRIPT_CIDX 0
    #define TRANSFORM_CIDX 1
    #define MESH_INSTANCE_CIDX 2
    #define CAMERA_CIDX 3
    #define UUID_CIDX 4

    #define CODE_ID 0
    #define TRANSFORM_ID 1


    struct TransformComponent
    {
        glm::vec3 Translation;
        glm::vec3 Rotation;
        glm::vec3 Scale;

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;

        operator glm::mat4 ()
        {
            glm::quat q = glm::quat(glm::radians(Rotation));
            glm::mat4 rotation = glm::toMat4(q);

            return glm::translate(glm::mat4(1.0f), Translation)
                * rotation
                * glm::scale(glm::mat4(1.0f), Scale);
        }

        void Translate(glm::vec3 translation)
        {
            glm::quat q = glm::quat(glm::radians(Rotation));
            Translation += q * translation;
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
        glm::vec3 Up = {0.0f, 1.f, 0.0f};

        float FOV;
        uint32_t Width;
        uint32_t Height;

        bool IsMain;
        bool IsAdjustable = true;
    };

    struct MeshInstance
    {
        Mesh *PMesh = nullptr;
        bool IsVisible;
    };

    struct UUIDComponent
    {
        UUID Id;
        std::string Name;
        UUIDComponent()
            : Id(), Name("New Object")
        {}
        UUIDComponent(const std::string &name)
            : Id(), Name(name)
        {}
        ~UUIDComponent() {}

        operator UUID() const {return Id;}
    };
}
