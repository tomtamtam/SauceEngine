#pragma once

#include "ECS/Entity.h"
#include "Input/InputSystem.h"
#include <memory>

namespace Sauce
{
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

        std::shared_ptr<InputSystem> Input;
    private:
        Entity m_Entity;
        friend class Scene;
    };
}
