#include "Core/UUID.h"
#include <bitset>
#include <cstddef>
#include <cstdint>
#include <string>
#include <sys/types.h>
#include <unordered_map>
#include <vector>

#define MAX_COMPONENTS 32
#define MAX_ENTITIES 500
typedef std::bitset<MAX_COMPONENTS> ComponentMask;


namespace Sauce
{
    extern uint32_t s_ComponentCounter;

    class Scene
    {
    public:
        Scene(std::string name);
        Scene(UUID id);
        uint32_t AddEntity();

        template<typename T>
        T* AttachComponent(uint32_t enttID)
        {
            uint32_t componentId = GetComponentID<T>();
            if(m_ComponentPools.size() < componentId)
            {
                m_ComponentPools.resize(componentId + 1, nullptr);
            }
            if(m_ComponentPools[componentId] == nullptr)
            {
                m_ComponentPools[componentId] = new ComponentPool(sizeof(T));
            }

            T* component = new (m_ComponentPools[componentId]->get(enttID)) T();
            
            m_Entities[componentId].Mask.set(componentId);
            m_Entities[componentId].ID = componentId;

            return component;
        }

        template<typename T>
        void RemoveComponent(uint32_t id)
        {
            uint32_t componentId = GetComponentID<T>();
            m_Entities[componentId].Mask.reset(componentId);
            m_Entities[componentId].ID = componentId;
        }

        template<typename T>
        T* GetComponent(uint32_t enttID)
        {
            uint32_t componentID = GetComponentID<T>();

            if(!m_Entities[enttID].Mask.test(componentID))
            {
                return nullptr;
            }

            T* component = static_cast<T>(m_ComponentPools[componentID]->get(enttID));
            return component;
        }

    private:
        struct EntityDesc
        {
            UUID ID;
            ComponentMask Mask;
        };

        struct ComponentPool
        {
            ComponentPool(size_t elementsize)
            {
                ElementSize = elementsize;
                m_Data = new char[ElementSize * MAX_ENTITIES];
            }

            ~ComponentPool()
            {
                delete[] m_Data;
            }

            inline void *get(size_t index)
            {
                return m_Data + index * ElementSize;
            }
        
            size_t ElementSize { 0 };

        private:
            char *m_Data { nullptr };
        };

        template<typename T>
        uint32_t GetComponentID()
        {
            static uint32_t s_ComponentID = s_ComponentCounter++; //static means 1 per T
            return s_ComponentID;
        }

        UUID m_ID;
        std::string m_Name;

        std::vector<EntityDesc> m_Entities;
        std::vector<ComponentPool*> m_ComponentPools;
    };
}
