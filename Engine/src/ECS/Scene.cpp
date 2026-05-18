#include "Scene.h"
#include "Entity.h"

namespace Sauce
{
    Scene::Scene(UUID &uuid, const std::string &name)
        : m_Name(name), m_ID(uuid)
    {
        entt::entity entity = m_Registry.create();
    }

    Scene::~Scene()
    {}

    Entity Scene::CreateEntity()
    {
        return { m_Registry.create(), this, UUID(), "NewEntity"};
    }

    Entity Scene::CreateEntity(const std::string &name)
    {
        return { m_Registry.create(), this, UUID(), name};
    }

    Entity Scene::CreateEntity(const std::string &name, UUID uuid)
    {
        return { m_Registry.create(), this, uuid, name};
    }
}