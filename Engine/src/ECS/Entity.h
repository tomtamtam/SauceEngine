#pragma once

#include "Core/EntryPoint.h"
#include "Core/UUID.h"
namespace Sauce
{
    class Entity
    {
    public:
        template<typename T>
        T* GetComponent()
        {
            return s_App->GetCurrentScene()->GetComponent<T>(m_UUID);
        }

        template<typename T>
        void AttachComponent()
        {
            s_App->GetCurrentScene()->AttachComponent<T>(m_UUID);
        }

        template<typename T>
        void RemoveComponent()
        {
            s_App->GetCurrentScene()->RemoveComponent<T>(m_UUID);
        }

        inline UUID GetUUID() const
        {
            return m_UUID;
        }

        virtual void Update();
        virtual void Start();
        virtual void OnDestroy();
    private:
        UUID m_UUID;
    };
}
