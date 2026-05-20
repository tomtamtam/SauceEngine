#include "ECS/Entity.h"

namespace Sauce
{
    class ScriptableEntity
    {
    public:
        virtual ~ScriptableEntity() = default;

        virtual void OnInit(){}
        virtual void OnStart(){}
        virtual void OnUpdate(){}
        virtual void OnDestroy(){}

        template<typename T>
        T* GetComponent()
        {
            return m_Entity.GetComponent<T>();
        }

        template<typename T>
        bool HasComponent()
        {
            return m_Entity.HasComponent<T>();
        }
        
    private:
        Entity m_Entity;
        friend class Scene;
    };
}