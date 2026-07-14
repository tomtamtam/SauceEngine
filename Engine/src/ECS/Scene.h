#pragma once

#include "Core/UUID.h"
#include "Render/Renderer.h"
#include "Input/InputSystem.h"
#include "entt/entity/fwd.hpp"
#include "entt/entt.hpp"
#include <cstdint>
#include <memory>
#include "Core/SceneSerializer.h"

#define F_NEAR 1.0f
#define F_FAR 1000.0f

namespace Sauce
{
    class Entity;

    class Scene
    {
    public:
        Scene(std::shared_ptr<Renderer> renderer);
        ~Scene();

        Entity CreateEntity();
        Entity CreateEntity(const std::string &name);

        void Init();
        void Start();
        void Update(float dt);

        std::shared_ptr<InputSystem> GetInputSystem();

        void OnResize(uint32_t width, uint32_t height);

    private:
        entt::registry m_Registry;
        UUID m_ID;
        std::string m_Name;
        std::shared_ptr<Renderer> m_Renderer;
        std::shared_ptr<InputSystem> m_InputSystem;
        uint32_t m_WindowWidth, m_WindowHeight;

        friend class Entity;
        friend class SceneSerializer;
    };
}
