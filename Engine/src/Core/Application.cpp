#include "Application.h"
#include <array>
#include <cassert>
#include <cstdint>
#include <dlfcn.h>
#include <memory>
#include <string>
#include "Core/ScriptEngine.h"
#include "ECS/Components.h"
#include "ECS/Scene.h"
#include "GLFW/glfw3.h"
#include "Render/Shader.h"
#include "Util/Json.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "nlohmann/json.hpp"
#include "ECS/Entity.h"


namespace Sauce
{
    Application::Application()
        : m_IsRunning(true)
    {
    }

    Application::~Application()
    {
    }


    void Application::Setup()
    {
        auto self = shared_from_this();
        auto onShouldClose = [self](){self->Terminate();};
        m_CurrentWindow = {"test", 800, 800, onShouldClose};
        m_CurrentWindow.Init();

        m_CurrentScene = LoadScene("../Game/Scenes/TestScene.json");
        m_CurrentScene->Init();


        Shader shader("../Game/Shaders/Default.glsl");
    }

    void Application::Terminate()
    {
        m_IsRunning = false;
    }


    void Application::Run()
    {
        Setup();

        m_BeginTime = glfwGetTime();
        m_EndTime = glfwGetTime();
        m_DT = -1.0f;

        m_CurrentScene->Start();

        while (m_IsRunning)
        {
            m_CurrentWindow.Update();

            if(m_DT >= 0.0f)
            {
                m_CurrentScene->Update();
            }

            m_EndTime = glfwGetTime();
            m_DT = m_EndTime - m_BeginTime;
            m_BeginTime = m_EndTime;
        }

        m_CurrentWindow.Destroy();
    }

    glm::mat4 GetTransformFromJson(json component)
    {
        json values = component["values"];
        glm::mat4 transform = glm::mat4(1.0f);

        auto [px, py, pz, rx, ry, rz, sx, sy, sz] = values.get<std::array<float, 9>>();

        //position
        transform = glm::translate(transform, glm::vec3(px, py, pz));
        
        //rotation
        transform = glm::rotate(transform, glm::radians(rx), glm::vec3(1, 0, 0));
        transform = glm::rotate(transform, glm::radians(ry), glm::vec3(0, 1, 0));
        transform = glm::rotate(transform, glm::radians(rz), glm::vec3(0, 0, 1));

        //scale
        transform = glm::scale(transform, glm::vec3(sx, sy, sz));

        return transform;
    }

    Script LoadScriptFromJson(json j)
    {
        std::string name = j["values"][0];
        Script t = ScriptEngine::AttachScript(ScriptEngine::LoadScript(name, "./libGame.so"), name);
        return t;
    }




    std::shared_ptr<Scene> Application::LoadScene(const std::string &path)
    {
        json j = ParseJsonFromFile(path);
        std::string name = j["name"];
        UUID uuid = (UUID)j["id"].get<uint64_t>();

        auto scene = std::make_shared<Scene>(uuid, name);


        //objects
        uint32_t nObjects = j["objects"].size();


        for(int i = 0; i < nObjects; i++)
        {
            json obj = j["objects"][i];
            std::string objName = obj["name"];
            UUID objUUID = (UUID)obj["id"].get<uint64_t>();

            Entity ent(scene->CreateEntity(objName, objUUID));


            //components
            uint32_t nComponents = obj["components"].size();

            for(int j = 0; j < nComponents; j++)
            {
                json component = obj["components"][j];
                uint32_t id = component["id"];

                switch (id)
                {
                case 1:
                    ent.AddComponent<TransformComponent>();
                    ent.GetComponent<TransformComponent>() = GetTransformFromJson(component);
                    break;
                case 0:
                    ent.AddComponent<Script>();
                    ent.GetComponent<Script>() = LoadScriptFromJson(component);
                    break;
                }
            }
        }
        return scene;
    }
}
