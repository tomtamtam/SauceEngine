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
#include "Util/Json.h"

namespace Sauce
{
    Application::Application()
        : m_IsRunning(true), m_CurrentScene("Main Scene")
    {
    }


    void Application::Setup()
    {
        auto self = shared_from_this();
        auto onShouldClose = [self](){self->Terminate();};
        m_CurrentWindow = {"test", 800, 800, onShouldClose};
        m_CurrentWindow.Init();

        m_CurrentScene = LoadScene("../Game/Scenes/TestScene.json");
    }

    void Application::Terminate()
    {
        m_IsRunning = false;
    }


    void Application::Run()
    {
        Setup();
        std::cout << "finished\n";

        while (m_IsRunning)
        {
          m_CurrentWindow.Update();
        }

        m_CurrentWindow.Destroy();
    }

    Scene LoadScene(const std::string &path)
    {
        json jScene = ParseJsonFromFile(path);
        uint64_t id = jScene["id"].get<uint64_t>();
        Scene scene((UUID) id);
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
                    scene.GetComponent<Code>(id)->Start();
                    break;
                }
                case TRANSFORM_ID:
                {
                    scene.AttachComponent<Transform>(id);
                    Transform t = GetTransformFromJson(comp);
                    scene.GetComponent<Transform>(id)->Position = t.Position;
                    scene.GetComponent<Transform>(id)->Scale = t.Scale;
                    scene.GetComponent<Transform>(id)->Rotation = t.Rotation;
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
        std::string dllName = j["name"];
        dllName = "lib" + dllName + ".so";


        std::string dllPath = ENTITY_DLL_PATH + '/' + dllName;
        void* h = dlopen(dllPath.c_str(), RTLD_NOW);

        if(!h)
        {
            std::cerr << "dlopen: " << dlerror() << '\n';
            assert(false);
        }

        using FactorryStart = void(*)();

        auto start = (FactorryStart) dlsym(h, "Start");

        if(!start)
        {
            std::cerr << "start factorry returned nullptr\n";
            assert(false);
        }

        using FactorryUpdate = void(*)(float deltaTime);

        auto update = (FactorryUpdate) dlsym(h, "Update");

        if(!update)
        {
            std::cerr << "update factorry returned nullptr\n";
            assert(false);
        }

        using FactorryonDestroy = void(*)();
        auto onDestroy = (FactorryonDestroy) dlsym(h, "OnDestroy");

        if(!onDestroy)
        {
            std::cerr << "destroy factorry returned nullptr\n";
            assert(false);
        }

        Code code;
        code.Start = start;
        code.Update = update;
        code.OnDestroy = onDestroy;

        return code;
    }
}
