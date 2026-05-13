#pragma once

#include "Core/UUID.h"
#include "Render/Window.h"
#include "ECS/Scene.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <cstdint>

namespace Sauce
{
    class Application : public std::enable_shared_from_this<Application>
    {
    public:

        Application();
        virtual ~Application() = default;

        void Run();
        void Terminate();

        UUID AddScene(UUID uuid);
        void RemoveScene(UUID uuid);
        void SwitchScene(UUID uuid);

    private:
        const std::string ENTITY_DLL_PATH = ".";
        void Setup();

        void LoadScene(const std::string &path, Scene *scene);
        void LoadEntity(const std::string &dllName, UUID uuid);

        bool m_IsRunning;
        std::string m_Name;

        Window m_CurrentWindow;

        Scene m_CurrentScene;
    };

    Application *CreateApplication();
}

extern Sauce::Application *s_App;