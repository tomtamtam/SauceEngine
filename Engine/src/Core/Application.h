#pragma once

#include "Core/UUID.h"
#include "ECS/Components.h"
#include "Render/Window.h"
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

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

        UUID AddScene(Scene scene);
        void RemoveScene(UUID uuid);
        void SwitchScene(UUID uuid);

        Scene *GetCurrentScene()
        {
            return m_CurrentScene;
        }

        UUID GetSceneByName(const std::string &name);
        void ChangeSceneName(UUID uuid, const std::string &newName);
    private:
        const std::string ENTITY_DLL_PATH = ".";
        void Setup();


        float m_DT, m_BeginTime, m_EndTime;

        bool m_IsRunning;
        std::string m_Name;

        Window m_CurrentWindow;

        Scene *m_CurrentScene;
        std::unordered_map<std::string, UUID> m_SceneNames;
        std::unordered_map<UUID, Scene*> m_Scenes;
    };

    Scene LoadScene(const std::string &path);

    Application *CreateApplication();
    Transform GetTransformFromJson(json comp);
    Code GetCodeFromJson(json j, UUID uuid, Scene *scene);
}
