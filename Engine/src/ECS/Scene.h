#pragma once

#include "Core/UUID.h"
#include "Render/Renderer.h"
#include "Input/InputSystem.h"
#include "entt/entity/fwd.hpp"
#include "entt/entt.hpp"
#include <memory>
#include <string>

#define F_NEAR 1.0f
#define F_FAR 1000.0f

namespace Sauce
{
    class Entity;

    class Scene
    {
    public:
        Scene(const UUID &uuid, const std::string &name, std::shared_ptr<Renderer> renderer);
        ~Scene();

        Entity CreateEntity();
        Entity CreateEntity(const std::string &name);
        Entity CreateEntity(const std::string &name, UUID uuid);

        void Init();
        void Start();
        void Update(float dt);

        std::shared_ptr<InputSystem> GetInputSystem();

    private:
        entt::registry m_Registry;
        UUID m_ID;
        std::string m_Name;
        std::shared_ptr<Renderer> m_Renderer;
        std::shared_ptr<InputSystem> m_InputSystem;

        friend class Entity;
    };
}
