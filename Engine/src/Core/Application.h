#pragma once

#include <nlohmann/json.hpp>

#include <memory>
#include <string>

#include "Core/SceneSerializer.h"

#include "ECS/Scene.h"

using json = nlohmann::json;

namespace Sauce
{
    class Application : public std::enable_shared_from_this<Application>
    {
    public:
        Application();
        virtual ~Application();

        void Run();
        void Terminate();

        void SwitchScene();

    private:
        const std::string ENTITY_DLL_PATH = "./libGame.so";
        void Setup();

        float m_DT, m_BeginTime, m_EndTime;

        bool m_IsRunning;
        std::string m_Name;
        std::string m_DefaultScene;
        std::string m_DefaultShader;

        void *m_HandleGameDll;

        std::shared_ptr<IRenderer> m_Renderer;

        std::shared_ptr<Scene> m_CurrentScene;
		std::shared_ptr<InputSystem> m_InputSystem;
		std::unique_ptr<Shader> m_Shader;
        SceneSerializer m_SceneSerializer;
    };
}
