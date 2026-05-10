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

    uint32_t Scene::AddEntity() 
    {
        std::cout << "added entity\n";

        m_Entities.push_back({m_Entities.size(), ComponentMask()});
        return m_Entities.back().ID;
    }
}
