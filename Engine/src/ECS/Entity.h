#pragma once

#include <utility>

#include "ECS/Scene.h"

#include "entt/entity/fwd.hpp"

namespace Sauce
{
    class Entity
    {
    public:

        Entity(entt::entity handle, Scene *scene);
        Entity();
        Entity(const Entity &other) = default;

        template<typename T>
        bool HasComponent()
        {
            return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
        }

        template<typename T, typename... Args>
        void AddComponent(Args&&... args)
        {
            m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
        }

        template<typename T>
        T &GetComponent()
        {
            return m_Scene->m_Registry.get<T>(m_EntityHandle);
        }

    private:
        entt::entity m_EntityHandle;
        Scene* m_Scene;
    };
}
