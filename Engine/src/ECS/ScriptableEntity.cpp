#include "ScriptableEntity.h"

#include "ECS/Entity.h"

namespace Sauce
{
	std::shared_ptr<Entity> ScriptableEntity::CreateEntity()
	{
		return m_Entity.m_Scene->CreateEntity();
	}

	std::shared_ptr<Entity> ScriptableEntity::CreateEntity(const std::string &name)
	{
		return m_Entity.m_Scene->CreateEntity(name);
	}
}
