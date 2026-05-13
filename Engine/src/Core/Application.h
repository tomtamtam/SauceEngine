#pragma once

#include "Core/UUID.h"
#include "ECS/Components.h"
#include "Render/Window.h"
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
        virtual ~Application() = default;

        void Run();
        void Terminate();

        UUID AddScene(UUID uuid);
        void RemoveScene(UUID uuid);
        void SwitchScene(UUID uuid);

    private:
        const std::string ENTITY_DLL_PATH = ".";
        void Setup();




        bool m_IsRunning;
        std::string m_Name;

        Window m_CurrentWindow;

        Scene m_CurrentScene;
    };

    Scene LoadScene(const std::string &path);

    Application *CreateApplication();
    Transform GetTransformFromJson(json comp);
    Code GetCodeFromJson(json j, UUID uuid, Scene *scene);
}

extern Sauce::Application *s_App;
