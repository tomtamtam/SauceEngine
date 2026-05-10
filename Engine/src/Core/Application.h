#pragma once

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

    private:
        void Setup();

        bool m_IsRunning;
        std::string m_Name;

        Window m_CurrentWindow;

        Scene m_CurrentScene;
    };
}
