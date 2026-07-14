#include "Entity.h"
#include "ECS/Components.h"
#include "entt/entity/fwd.hpp"

namespace Sauce
{
    Entity::Entity(entt::entity handle, Scene *scene)
        : m_EntityHandle(handle), m_Scene(scene)
    {
        AddComponent<UUIDComponent>();
    }

    Entity::Entity() = default;
}
