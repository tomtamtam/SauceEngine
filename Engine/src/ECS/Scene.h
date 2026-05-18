#pragma once

#include "Core/UUID.h"
#include "entt/entity/fwd.hpp"
#include "entt/entt.hpp"
#include <string>

namespace Sauce
{
    class Entity;
    
    class Scene
    {
    public:
        Scene(UUID &uuid, const std::string &name);
        ~Scene();

        Entity CreateEntity();
        Entity CreateEntity(const std::string &name);
        Entity CreateEntity(const std::string &name, UUID uuid);
    
    private:
        entt::registry m_Registry;
        UUID m_ID;
        std::string m_Name;

        friend class Entity;
    };
}