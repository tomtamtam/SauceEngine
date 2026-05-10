#include "Scene.h"
#include "Core/UUID.h"
#include "ECS/Entity.h"
#include "Render/Viewport.h"
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>

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

    uint64_t Scene::AddEntity() 
    {
        if(!m_FreeEntities.empty())
        {
            uint32_t newIdx = m_FreeEntities.back();
            m_FreeEntities.pop_back();
            uint32_t newID = CreateEntityID(newIdx, GetEntityVersion(m_Entities[newIdx].ID));
            m_Entities[newIdx].ID = newID;
            std::cout << "new entt at idx: " << newIdx << '\n';
            return m_Entities[newIdx].ID;
        }
        m_Entities.push_back({CreateEntityID(m_Entities.size(), 0), ComponentMask()});
        std::cout << "new entt at idx: " << GetEntityIndex(m_Entities.size()) << '\n';
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
}
