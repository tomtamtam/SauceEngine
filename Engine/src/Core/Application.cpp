#include "Application.h"
#include "Core/UUID.h"
#include "ECS/Components.h"
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <dlfcn.h>
#include <iostream>
#include <memory>
#include "ECS/Entity.h"
#include "GLFW/glfw3.h"
#include "Render/Shader.h"
#include "Util/Json.h"

namespace Sauce
{
    Application::Application()
        : m_IsRunning(true), m_CurrentScene(nullptr)
    {
        m_HandleGameDll = dlopen(ENTITY_DLL_PATH.c_str(), RTLD_NOW);
        if(!m_HandleGameDll)
        {
            std::cerr << "Failed to open libGame dll\n";
        }
    }

    Application::~Application()
    {
        delete m_CurrentScene;
        for (auto &pair : m_Scenes)
            delete pair.second;
    }


    void Application::Setup()
    {
        auto self = shared_from_this();
        auto onShouldClose = [self](){self->Terminate();};
        m_CurrentWindow = {"test", 800, 800, onShouldClose};
        m_CurrentWindow.Init();

        auto mainScene = AddScene(LoadScene("../Game/Scenes/TestScene.json"));

        SwitchScene(mainScene);

        Shader shader("../Game/Shaders/Default.glsl");
    }

    UUID Application::AddScene(Scene scene)
    {
        m_Scenes[scene.GetUUID()] = &scene;
        return scene.GetUUID();
    }

    void Application::RemoveScene(UUID uuid)
    {
        m_Scenes[uuid] = nullptr;
    }

    void Application::SwitchScene(UUID uuid)
    {
        m_CurrentScene = m_Scenes[uuid];
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

        while (m_IsRunning)
        {
          m_CurrentWindow.Update();

          if(m_DT >= 0.0f)
          {
              //update scene ect
          }

          m_EndTime = glfwGetTime();
          m_DT = m_EndTime - m_BeginTime;
          m_BeginTime = m_EndTime;
        }

        m_CurrentWindow.Destroy();
    }

    Scene LoadScene(const std::string &path)
    {
        json jScene = ParseJsonFromFile(path);
        uint64_t id = jScene["id"].get<uint64_t>();
        Scene scene((UUID) id);
        scene.SetName(jScene["name"]);
        size_t nObjects = jScene["objects"].size();
        for(int i = 0; i < nObjects; i++)
        {
            uint64_t id = jScene["objects"][i]["id"].get<uint64_t>();
            scene.AddEntityUUID((UUID)id);
            size_t nComponents = jScene["objects"][i]["components"].size();
            for(int n = 0; n < nComponents; n++)
            {
                if(!jScene["objects"][i]["components"][n]["id"].is_number_integer())
                {
                    std::cerr << "component id is not of type integer\n";
                    assert(false);
                }

                auto &comp = jScene["objects"][i]["components"][n];
                switch(comp["id"].get<int>())
                {
                case CODE_ID:
                {
                    scene.AttachComponent<Code>(id);
                    json &obj = jScene["objects"][i];
                    Code c = GetCodeFromJson(obj, id, &scene);
                    scene.GetComponent<Code>(id)->OnDestroy = c.OnDestroy;
                    scene.GetComponent<Code>(id)->Start = c.Start;
                    scene.GetComponent<Code>(id)->Update = c.Update;
                    break;
                }
                case TRANSFORM_ID:
                {
                    scene.AttachComponent<Transform>(id);
                    Transform t = GetTransformFromJson(comp);
                    scene.GetComponent<Transform>(id)->Position = t.Position;
                    scene.GetComponent<Transform>(id)->Scale = t.Scale;
                    scene.GetComponent<Transform>(id)->Rotation = t.Rotation;
                    break;
                }
                default:
                    break;
                }
            }
        }
        return scene;
    }

    Application *CreateApplication()
    {
        return new Application();
    }

    int getComponentIdx(int component, json obj)
    {
        int nComponents = obj["components"].size();
        for(int i = 0; i < nComponents; i++)
        {
            if(obj["component"][i]["id"].get<int>() == component)
                return i;
        }

        std::cerr << "component '" << component << "' does not exist in obj " << obj.dump() << '\n';
        assert(false);
        return -1;
    }

    Transform GetTransformFromJson(json comp)
    {
        auto transformObj = comp["values"];
        Transform t;
        t.Position.x = transformObj[0];
        t.Position.y = transformObj[1];
        t.Position.z = transformObj[2];

        t.Scale.x = transformObj[3];
        t.Scale.y = transformObj[4];
        t.Scale.z = transformObj[5];

        t.Rotation.x = transformObj[6];
        t.Rotation.y = transformObj[7];
        t.Rotation.z = transformObj[8];

        return t;
    }

    Code GetCodeFromJson(json j, UUID uuid, Scene *scene)
    {
        std::string oName = j["name"];
        std::string dllName = "lib" + oName + ".so";

        using FactorryEntity = Entity*(*)(UUID uuid);
        std::string oFuncName = "Get" + oName;
        std::cout << "ofuncname: " << oFuncName << '\n';
        void *h = dlopen("./libGame.so", RTLD_NOW);
        auto eFunc = (FactorryEntity) dlsym(h, oFuncName.c_str());
        if(!eFunc)
        {
            std::cerr << "Entity factorry returned nullptr\n";
            assert(false);
        }
        std::cout << "e\n";
        auto e = eFunc(uuid);
        std::cout << "e\n";
        Code c;
        c.Start     = [e](){ e->Start(); };
        c.Update    = [e](){ e->Update(); };
        c.OnDestroy = [e](){ e->OnDestroy(); };
        std::cout << "exit code\n";
        return c;
    }


    UUID Application::GetSceneByName(const std::string &name)
    {
        return m_SceneNames[name];
    }

    void Application::ChangeSceneName(UUID uuid, const std::string &newName)
    {
        m_SceneNames.at(newName) = uuid;
    }
}
