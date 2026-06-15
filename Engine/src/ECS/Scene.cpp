#include "Scene.h"
#include "ECS/Components.h"
#include "Entity.h"
#include "Render/Renderer.h"
#include "glm/ext/matrix_float4x4.hpp"
#include <memory>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Sauce
{
    Scene::Scene(const UUID &uuid, const std::string &name, std::shared_ptr<Renderer> renderer)
        : m_Name(name), m_ID(uuid), m_Renderer(renderer)
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
            {
                script.Instance->m_Entity = Entity(entity, this, UUID(), script.ScriptName);
                script.Instance->OnInit();
            }
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
        m_Renderer->Clear();

        //Script Update
        auto scriptView = m_Registry.view<Script>();
        for(auto entity : scriptView)
        {
            scriptView->get(entity).Instance->OnUpdate();
        }

        //main cam
        glm::mat4 cam;
        glm::mat4 camTransform;
        auto camView = m_Registry.view<CameraComponent>();
        for(auto entity : camView)
        {
            if(camView->get(entity).IsMain)
            {
                cam = camView->get(entity);
                camTransform = m_Registry.get<TransformComponent>(entity);
            }
        }

        //meshes
        auto meshView = m_Registry.view<MeshInstance>();
        for(auto entity : meshView)
        {
            m_Renderer->Submit(m_Registry.get<TransformComponent>(entity), cam, camTransform, meshView->get(entity).PMesh);
        }

        m_Renderer->Draw();
    }
}
