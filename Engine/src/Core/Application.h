#pragma once

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

        std::shared_ptr<Scene> LoadScene(const std::string &path);

        float m_DT, m_BeginTime, m_EndTime;

        bool m_IsRunning;
        std::string m_Name;

        void *m_HandleGameDll;

        std::unique_ptr<Renderer> m_Renderer;

        std::shared_ptr<Scene> m_CurrentScene;
    };
}
