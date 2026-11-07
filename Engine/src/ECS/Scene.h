#pragma once

#include <cstdint>

#include <memory>

#include "Core/AssetSystem.h"
#include "Core/UUID.h"

#include "Input/InputSystem.h"

#include "Render/IRenderer.h"

#include "entt/entt.hpp"

#include "entt/entity/fwd.hpp"

#include "nlohmann/json_fwd.hpp"

constexpr float F_NEAR {1.0f};
constexpr float F_FAR {1000.0f};

using json = nlohmann::json;

class Editor;
class EditorApp;

namespace Sauce
{
    class Application;
    class Entity;
    class ScriptableEntity;

    class Scene
    {
    public:
        Scene(std::shared_ptr<IRenderer> renderer, std::shared_ptr<InputSystem> inputSystem);
        ~Scene();

		std::shared_ptr<Entity> CreateEntity();
		std::shared_ptr<Entity> CreateEntity(const std::string &name);

        void Init();
        void Start();
        void Update(float dt);

        std::shared_ptr<InputSystem> GetInputSystem();
        void SetInputSystem(std::shared_ptr<InputSystem> inputSystem);

        void OnResize(uint32_t width, uint32_t height);

		std::optional<AssetSystem> p_AssetSystem;
        entt::registry m_Registry;
    private:
        UUID m_ID;
        std::string m_Name;
        std::shared_ptr<IRenderer> m_Renderer;
        std::shared_ptr<InputSystem> m_InputSystem;
        uint32_t m_WindowWidth, m_WindowHeight;
        json m_Assets;

        friend class SceneSerializer;
    };
}
