//noincludeformat
#pragma once
#include <entt/entity/fwd.hpp>

#include "ECS/Scene.h"
#include "ECS/Components.h"


#include "entt/entity/fwd.hpp"

namespace Sauce
{
	class ScriptableEntity;
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
            m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandle, args...);
        }

        template<typename T>
        T &GetComponent()
        {
            return m_Scene->m_Registry.get<T>(m_EntityHandle);
        }

        template<typename T>
        void RemoveComponent()
        {
            return m_Scene->m_Registry.remove<T>(m_EntityHandle);
        }

        inline TransformComponent &Transform() { return GetComponent<TransformComponent>(); }
		std::string &Name();

    private:
        entt::entity m_EntityHandle;
        Scene* m_Scene;

        friend class ScriptableEntity;
    };
}
