#pragma once

#include "Core/SceneSerializer.h"
#include "ECS/Scene.h"
#include "Render/Renderer.h"
#include <memory>
#include <nlohmann/json.hpp>
#include <string>

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

        std::shared_ptr<Renderer> m_Renderer;

        std::shared_ptr<Scene> m_CurrentScene;
        SceneSerializer m_SceneSerializer;
    };
}
