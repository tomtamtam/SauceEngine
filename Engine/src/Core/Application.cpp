#include "Application.h"
#include <array>
#include <cassert>
#include <cstdint>
#include <dlfcn.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include "Core/ScriptEngine.h"
#include "ECS/Components.h"
#include "ECS/Scene.h"
#include "GLFW/glfw3.h"
#include "Render/Mesh.h"
#include "Render/Renderer.h"
#include "Render/Shader.h"
#include "Util/Json.h"
#include "glm/fwd.hpp"
#include "nlohmann/json.hpp"
#include "ECS/Entity.h"


namespace Sauce
{
    Application::Application()
        : m_IsRunning(true), m_Renderer(nullptr)
    {
    }

    Application::~Application()
    {
    }


    void Application::Setup()
    {

        //Load projinfo
        std::ifstream info("../.project_info.json");
        json jinfo;
        info >> jinfo;

        m_Name = jinfo["name"];
        m_DefaultScene = jinfo["defaultScene"];
        m_DefaultShader = jinfo["defaultShader"];

        auto self = shared_from_this();

        auto onShouldClose = [self](){self->Terminate();};
        auto onResizeCallback = [self](uint32_t width, uint32_t height) {};
        auto onKeyCallback = [self](GLFWwindow *window, int key, int scancode, int action, int mods) {self->m_CurrentScene->GetInputSystem()->KeyCallback(window, key, scancode, action, mods);};

        m_Renderer = std::make_unique<Renderer>();
        m_Renderer->CreateWindow(m_Name, 800, 800, onShouldClose, onKeyCallback);

        m_CurrentScene = LoadScene("../"+m_DefaultScene);
        m_CurrentScene->Init();

        auto shader = std::make_shared<Shader>("../"+m_DefaultShader);
        m_Renderer->SetMainShader(shader);
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
            if(m_DT >= 0.0f)
            {
                m_CurrentScene->Update(m_DT);
            }

            m_EndTime = glfwGetTime();
            m_DT = m_EndTime - m_BeginTime;
            m_BeginTime = m_EndTime;
        }

        m_Renderer->KillWindow();
    }

    TransformComponent GetTransformFromJson(json component)
    {
        json values = component["values"];
        glm::mat4 transform = glm::mat4(1.0f);

        auto [px, py, pz, rx, ry, rz, sx, sy, sz] = values.get<std::array<float, 9>>();

        TransformComponent t;
        t.Translation = {px, py, pz};
        t.Rotation = {rx, ry, rz};
        t.Scale = {sx, sy, sz};

        return t;
    }

    Script LoadScriptFromJson(json j)
    {
        std::string name = j["values"][0];
        Script t = ScriptEngine::AttachScript(ScriptEngine::LoadScript(name, "./libGame.so"), name);
        return t;
    }

    MeshInstance LoadMeshInstanceFromJson(json j)
    {
        MeshInstance i;
        i.IsVisible = true;
        i.PMesh = CreateCubeMesh();
        return i;
    }

    CameraComponent LoadCamFromJson(json j)
    {
        CameraComponent c;
        c.IsMain = true;
        c.Width = 500;//j["values"][0];
        c.Height = 500;//j["values"][1];
        c.FOV = 60.0f;//j["values"][2];
        return c;
    }




    std::shared_ptr<Scene> Application::LoadScene(const std::string &path)
    {
        std::cout << "before objects\n";
        json j = ParseJsonFromFile(path);
        std::string name = j["name"];
        UUID uuid = (UUID)j["id"].get<uint64_t>();

        auto scene = std::make_shared<Scene>(uuid, name, m_Renderer);

        if(j["objects"].size() == 0)
        {
            std::cout << "o objects\n";
            return scene;
        }
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
                case SCRIPT_CIDX:
                    ent.AddComponent<Script>();
                    ent.GetComponent<Script>() = LoadScriptFromJson(component);
                    ent.GetComponent<Script>().Instance = ent.GetComponent<Script>().InitializeScript();
                    ent.GetComponent<Script>().Instance->Input = scene->GetInputSystem();
                    break;
                case TRANSFORM_CIDX:
                    ent.AddComponent<TransformComponent>();
                    ent.GetComponent<TransformComponent>() = GetTransformFromJson(component);
                    break;
                case MESH_INSTANCE_CIDX:
                    ent.AddComponent<MeshInstance>();
                    ent.GetComponent<MeshInstance>() = LoadMeshInstanceFromJson(component);
                    break;
                case CAMERA_CIDX:
                    ent.AddComponent<CameraComponent>();
                    ent.GetComponent<CameraComponent>() = LoadCamFromJson(component);
                    break;
                }
            }
        }
        return scene;
    }
}
