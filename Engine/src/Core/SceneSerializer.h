// noincludeformat

#pragma once

#include <nlohmann/json.hpp>

#include <memory>

#include "ECS/Scene.h"

#include "ECS/Entity.h"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace Sauce
{
    class Scene;
    class SceneSerializer
    {
    public:
        SceneSerializer(std::shared_ptr<Scene> scene);
        ~SceneSerializer();

		void AttachComponent(json component, Entity *entity);
        void Serialze(const std::string &filepath);
        void Deserialze(const std::string &filepath);
    private:
        std::shared_ptr<Scene> m_Scene;
    };
}
