#pragma once

#include <memory>
#include <string>

#include "Core/Log.h"

#include "ECS/Entity.h"

#include "Input/InputSystem.h"

namespace Sauce
{
	struct TransformComponent;
	class Scene;
    class ScriptableEntity
    {
    public:
        virtual ~ScriptableEntity() = default;

        virtual void OnInit(){}
        virtual void OnStart(){}
        virtual void OnUpdate(float dt){}
        virtual void OnDestroy(){}

        template<typename T>
        T &GetComponent()
        {
            return m_Entity.GetComponent<T>();
        }

        template<typename T>
        bool HasComponent()
        {
            return m_Entity.HasComponent<T>();
        }

        template<typename T, typename... Args>
        void AddComponent(Args&&... args)
        {
            m_Entity.AddComponent<T>(args...);
        }

        template<typename T>
        void RemoveComponent()
        {
        	m_Entity.RemoveComponent<T>();
        }

        std::shared_ptr<Entity> CreateEntity();
        std::shared_ptr<Entity> CreateEntity(const std::string &name);

        inline TransformComponent &Transform() { return m_Entity.Transform(); }
        inline std::shared_ptr<InputSystem> Input() { return m_InputSystem; }
        inline void DebugLog(const std::string &msg) { LOG_DEBUG(msg); }
    private:
        Entity m_Entity;
        std::shared_ptr<InputSystem> m_InputSystem;

        friend class Scene;
    };
}
