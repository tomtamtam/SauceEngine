#include <nlohmann/json.hpp>

#include <iostream>
#include <memory>
#include <string>

#include "Scene.h"

#include "Entity.h"

#include "ECS/Components.h"

#include "Render/Renderer.h"

#include "glm/trigonometric.hpp"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"

using json = nlohmann::json;

namespace Sauce
{
    Scene::Scene(std::shared_ptr<Renderer> renderer)
        : m_Renderer(renderer)
    {
        entt::entity entity = m_Registry.create();
        m_InputSystem = std::make_shared<InputSystem>();
    }

    Scene::~Scene()
    {}

    Entity Scene::CreateEntity()
    {
        return { m_Registry.create(), this};
    }

    Entity Scene::CreateEntity(const std::string &name)
    {
        Entity e = { m_Registry.create(), this};
        e.AddComponent<UUIDComponent>(name);
        return e;
    }

    void Scene::Init()
    {
        auto scriptView = m_Registry.view<Script>();
        for (auto entity : scriptView)
        {
            auto& script = scriptView.get<Script>(entity);

            if (script.Instance)
            {
                script.Instance->m_Entity = Entity(entity, this);
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

    void Scene::Update(float dt)
    {
        //Script Update
        auto scriptView = m_Registry.view<Script>();
        for(auto entity : scriptView)
        {
            scriptView->get(entity).Instance->OnUpdate(dt);
        }

        //main cam
        glm::mat4 view = {1.0f};
        glm::mat4 proj = {1.0f};
        uint32_t width = 0, height = 0;
        auto camView = m_Registry.view<CameraComponent>();
        for(auto entity : camView)
        {
            auto &cam = camView->get(entity);
            if(cam.IsAdjustable)
            {
                if(cam.Width != m_Renderer->GetWinWidth())
                {
                    width = m_Renderer->GetWinWidth();
                }

                if(cam.Height != m_Renderer->GetWinHeight())
                    height = m_Renderer->GetWinHeight();
            }

            if(cam.IsMain)
            {
                if(!m_Registry.all_of<TransformComponent>(entity))
                {
                    std::cerr << "Object with component <CameraComponent> has NO component of type <TransformComponent>.\n";
                }
                auto &transform = m_Registry.get<TransformComponent>(entity);
                view = glm::lookAt(transform.Translation, transform.Translation + transform.Rotation, cam.Up);
                proj = glm::perspective(glm::radians(cam.FOV), (float)width / (float)height, F_NEAR, F_FAR);
            }
        }

        //meshes
        auto meshView = m_Registry.view<MeshInstance>();
        for(auto entity : meshView)
        {
            if(!p_AssetSystem->p_Meshes.at(meshView->get(entity).meshId).mesh) continue;
            m_Renderer->Submit(m_Registry.get<TransformComponent>(entity), view, proj, *p_AssetSystem->p_Meshes[meshView->get(entity).meshId].mesh);
        }
    }

    std::shared_ptr<InputSystem> Scene::GetInputSystem()
    {
        return m_InputSystem;
    }
}
