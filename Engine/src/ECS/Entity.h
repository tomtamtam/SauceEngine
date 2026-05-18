#pragma once

#include "ECS/Scene.h"
#include "entt/entity/fwd.hpp"

namespace Sauce
{
    class Entity
    {
    public:

        Entity(entt::entity handle, Scene *scene, UUID uuid, std::string name);
        Entity(const Entity &other) = default;

        template<typename T>
        bool HasComponent()
        {
            return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
        }

        template<typename T, typename ... Args>
        void AddComponent()
        {
            m_Scene->m_Registry.emplace<T>(m_EntityHandle);
        }

        template<typename T>
        T GetComponent()
        {
            return m_Scene->m_Registry.get<T>(m_EntityHandle);
        }
        
    private:
        UUID m_ID;
        std::string m_Name;
        entt::entity m_EntityHandle;
        Scene* m_Scene;
    };
}