#include "Scene.h"
#include "Core/UUID.h"
#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>

namespace Sauce {

    uint32_t s_ComponentCounter = 0;

    Scene::Scene(std::string name)
    {
        m_ID = UUID();
        m_Name = name;
    }

    Scene::Scene(UUID id)
    {
        m_ID = id;
    }

    Scene::~Scene()
    {
        for(auto p : m_ComponentPools)
            delete p;
    }

    uint64_t Scene::AddEntity() 
    {
        if(!m_FreeEntities.empty())
        {
            uint32_t newIdx = m_FreeEntities.back();
            m_FreeEntities.pop_back();
            uint32_t newID = CreateEntityID(newIdx, GetEntityVersion(m_Entities[newIdx].ID));
            m_Entities[newIdx].ID = newID;
            std::cout << "added entity (idx) " << GetEntityIndex(newID) << '\n';
            return m_Entities[newIdx].ID;
        }
        m_Entities.push_back({CreateEntityID(m_Entities.size(), 0), ComponentMask()});
        std::cout << "added entity (idx) " << m_Entities.size() << '\n';
        return m_Entities.back().ID;
    }

    void Scene::DestroyEntity(uint64_t id)
    {
        if(m_Entities[GetEntityIndex(id)].ID != id)
            return;

        uint64_t newID = CreateEntityID(uint32_t(-1), GetEntityVersion(id) + 1);
        m_Entities[GetEntityIndex(id)].ID = newID;
        m_Entities[GetEntityIndex(id)].Mask.reset();
        m_FreeEntities.push_back(GetEntityIndex(id));
    }

    uint64_t Scene::EntityByUUID(UUID uuid)
    {
        return m_UUIDMap[uuid];
    }

    UUID Scene::AddEntityUUID()
    {
        uint64_t ent =  AddEntity();
        UUID uuid = {};
        m_UUIDMap[uuid] = ent;
        return uuid;
    }

    UUID Scene::AddEntityUUID(UUID uuid)
    {
        uint64_t ent = AddEntity();
        m_UUIDMap[uuid] = ent;
        return uuid;
    }


    inline std::string Scene::GetName() const
    {
        return m_Name;
    }

    void Scene::SetName(const std::string &name)
    { 
        m_Name = name;
    }
}
