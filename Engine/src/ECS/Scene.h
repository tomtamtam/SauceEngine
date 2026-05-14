#include "Core/UUID.h"
#include <bitset>
#include <cstddef>
#include <cstdint>
#include <iostream>
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
        ~Scene();
        uint64_t AddEntity();

        template<typename T>
        T* AttachComponent(UUID uuid)
        {
            uint64_t id = EntityByUUID(uuid);
            if(m_Entities[GetEntityIndex(id)].ID != id)
                return nullptr;

            int componentId = GetComponentID<T>();

            if (m_ComponentPools.size() <= componentId)
            {
                m_ComponentPools.resize(componentId + 1, nullptr);
            }
            if (m_ComponentPools[componentId] == nullptr)
            {
                m_ComponentPools[componentId] = new ComponentPool(sizeof(T));
            }

            T* pComponent = new (m_ComponentPools[componentId]->get(GetEntityIndex(id))) T();

            m_Entities[GetEntityIndex(id)].Mask.set(componentId);
            std::cout << "Attaching component at entt (id) " << id << " component (id) " << componentId << '\n';
            return pComponent;
        }

        template<typename T>
        void RemoveComponent(UUID uuid)
        {
            uint64_t id = EntityByUUID(uuid);
            if(m_Entities[GetEntityIndex(id)].ID != id)
                return; 
            uint32_t componentId = GetComponentID<T>();
            m_Entities[GetEntityIndex(id)].Mask.reset(componentId);
        }

        template<typename T>
        T* GetComponent(UUID uuid)
        {
            uint64_t enttID = EntityByUUID(uuid);
            if(m_Entities[GetEntityIndex(enttID)].ID != enttID)
                return nullptr;
            uint32_t componentID = GetComponentID<T>();

            if(!m_Entities[GetEntityIndex(enttID)].Mask.test(componentID))
            {
                return nullptr;
            }

            T* component = static_cast<T*>(m_ComponentPools[componentID]->get(GetEntityIndex(enttID)));
            return component;
        }

        void DestroyEntity(uint64_t id);

        UUID AddEntityUUID();
        UUID AddEntityUUID(UUID uuid);
        uint64_t EntityByUUID(UUID uuid);
        inline std::string GetName() const;
        void SetName(const std::string &name);
        inline UUID GetUUID() const;

    private:

        struct EntityDesc
        {
            uint64_t ID;
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

        inline uint64_t CreateEntityID(uint32_t index, uint32_t version)
        {
            return ((uint64_t)index << 32) | ((uint64_t)version);
        }

        inline uint32_t GetEntityIndex(uint64_t id)
        {
            return id >> 32;
        }

        inline uint32_t GetEntityVersion(uint64_t id)
        {
            return (uint32_t)id;
        }

        inline bool IsEntityValid(uint64_t id)
        {
            return (id >> 32) != uint32_t(-1);
        }

        #define INVALID_ENTITY CreateEntityID(EntityIndex(-1), 0)

        std::vector<uint32_t> m_FreeEntities;

        std::unordered_map<UUID, uint64_t> m_UUIDMap;

        UUID m_ID;
        std::string m_Name;

        std::vector<EntityDesc> m_Entities;
        std::vector<ComponentPool*> m_ComponentPools;
    };
}
