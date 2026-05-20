#pragma once

#include "ECS/Components.h"
#include "ECS/Scene.h"
#include "Render/Window.h"
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

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

        Window m_CurrentWindow;

        void *m_HandleGameDll;

        std::shared_ptr<Scene> m_CurrentScene;
    };
}
