#pragma once

#include "Core/SauceEngine.h"
#include "Core/UUID.h"

namespace Sauce
{
    class Entity
    {
    public:
        Entity();
        ~Entity();
        template<typename T>
        T* GetComponent()
        {
            return s_App->GetCurrentScene()->GetComponent<T>(ID);
        }

        template<typename T>
        void AttachComponent()
        {
            s_App->GetCurrentScene()->AttachComponent<T>(ID);
        }

        template<typename T>
        void RemoveComponent()
        {
            s_App->GetCurrentScene()->RemoveComponent<T>(ID);
        }

        virtual void Update();
        virtual void Start();
        virtual void OnDestroy();
        UUID ID;
    };
}
