#include "Entity.h"
#include "entt/entity/fwd.hpp"

namespace Sauce
{
    Entity::Entity(entt::entity handle, Scene *scene, UUID uuid, std::string name)
        : m_EntityHandle(handle), m_Scene(scene), m_ID(uuid), m_Name(name)
    {}

    Entity::Entity() = default;
}