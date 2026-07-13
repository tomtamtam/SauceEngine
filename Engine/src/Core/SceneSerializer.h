#pragma once

#include "ECS/Scene.h"
#include <memory>
namespace Sauce
{
    class SceneSerializer
    {
    public:
        SceneSerializer(std::shared_ptr<Scene> scene);
        ~SceneSerializer();

        void Serialze(const std::string &filepath);
        void Deserialze(const std::string &filepath);
    private:
        std::shared_ptr<Scene> m_Scene;
    };
}
