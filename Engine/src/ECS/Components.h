#pragma once

#include <cstdint>
#include <dlfcn.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <functional>
#include <memory>
#include <string>

#include "ScriptableEntity.h"

#include "Core/AssetSystem.h"
#include "Core/UUID.h"

#include "Render/Mesh.h"
#include "Render/Texture.h"

#include "glm/fwd.hpp"

#include "glm/ext/vector_float3.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Sauce
{
    constexpr uint32_t SCRIPT_CIDX {0};
    constexpr uint32_t TRANSFORM_CIDX {1};
    constexpr uint32_t MESH_INSTANCE_CIDX {2};
    constexpr uint32_t CAMERA_CIDX {3};
    constexpr uint32_t UUID_CIDX {4};
    constexpr uint32_t TEXTURE_CIDX {5};

    constexpr uint32_t CODE_ID {0};
    constexpr uint32_t TRANSFORM_ID {1};

    namespace VIDX
    {
    	namespace CTexture
    	{
    		constexpr uint32_t ID { 0 };
    	}

    	namespace CUUID
    	{
    		constexpr uint32_t ID { 0 };
    		constexpr uint32_t NAME { 1 };
    		constexpr uint32_t PARENT { 2 };
    		constexpr uint32_t CHILDEREN { 3 };
    	}

    	namespace CScript
    	{
    		constexpr uint32_t NAME { 0 };
    	}

    	namespace CMesh
    	{
    		constexpr uint32_t IS_VISIBLE { 0 };
    		constexpr uint32_t ID { 1 };
    	}
    	
    	namespace CCamera
    	{
    		constexpr uint32_t FOV { 0 };
    		constexpr uint32_t WIDHT { 1 };
    		constexpr uint32_t HEIGHT { 2 };
    		constexpr uint32_t IS_MAIN { 3 };
    		constexpr uint32_t IS_ADJUSTABLE { 4 };
    	}

    	namespace CTransform
    	{
    		constexpr uint32_t TRANSFORM_X { 0 };
    		constexpr uint32_t TRANSFORM_Y { 1 };
    		constexpr uint32_t TRANSFORM_Z { 2 };

    		constexpr uint32_t ROTATION_X {	3 };
    		constexpr uint32_t ROTATION_Y {	4 };
    		constexpr uint32_t ROTATION_Z {	5 };

    		constexpr uint32_t SCALE_X { 6 };
    		constexpr uint32_t SCALE_Y { 7 };
    		constexpr uint32_t SCALE_Z { 8 };
    	}
    }

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
		uint32_t meshId {};
        bool IsVisible { true };
    };

    struct UUIDComponent
    {
        UUID Id;
        std::string Name;
		std::optional<UUID> parent {};
		std::vector<UUID> childeren;

        UUIDComponent()
            : Id(), Name("New Object")
        {}
        UUIDComponent(const std::string &name)
            : Id(), Name(name)
        {}
        ~UUIDComponent() {}

        operator UUID() const {return Id;}
    };

    struct TextureComponent
    {
    	uint32_t textureId {};
    };
}
