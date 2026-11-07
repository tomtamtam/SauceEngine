
#include "Entity.h"

#include "Core/Log.h"

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

	std::string &Entity::Name()
	{
		if(!HasComponent<UUIDComponent>())
			LOG_ERROR("tryed to get name of entity that does not contain UUIDComponent");

		return GetComponent<UUIDComponent>().Name;
	}
}
