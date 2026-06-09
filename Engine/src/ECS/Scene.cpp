#include "Scene.h"
#include "ECS/Components.h"
#include "Entity.h"
#include "nlohmann/json.hpp"
#include <memory>

using json = nlohmann::json;

namespace Sauce
{
    Scene::Scene(const UUID &uuid, const std::string &name)
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

    void Scene::Init()
    {
        auto scriptView = m_Registry.view<Script>();
        for (auto entity : scriptView)
        {
            auto& script = scriptView.get<Script>(entity);

            script.Instance = script.InitializeScript();

            if (script.Instance)
                script.Instance->OnInit();
        }
    }

    void Scene::Start()
    {
        auto scriptView = m_Registry.view<Script>();
        for(auto entity : scriptView)
        {
            scriptView->get(entity).Instance->OnStart();
        }
    }

    void Scene::Update()
    {
        auto scriptView = m_Registry.view<Script>();
        for(auto entity : scriptView)
        {
            scriptView->get(entity).Instance->OnUpdate();
        }
    }
}
