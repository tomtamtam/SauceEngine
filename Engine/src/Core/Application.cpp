#include "Application.h"
#include "Core/UUID.h"
#include "ECS/Components.h"
#include <cassert>
#include <cstdint>
#include <dlfcn.h>
#include <iostream>
#include <memory>
#include "ECS/Entity.h"

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

        UUID e = m_CurrentScene.AddEntityUUID();
        m_CurrentScene.AttachComponent<Transform>(m_CurrentScene.EntityByUUID(e));
        std::cout << "finishedddd app setup\n";


        LoadEntity("libGame.so", UUID());
    }

    void Application::Terminate()
    {
        m_IsRunning = false;
    }


    void Application::Run()
    {
        Setup();
        
        while (m_IsRunning)
        {
          m_CurrentWindow.Update();
        }

        m_CurrentWindow.Destroy();
    }

    void Application::LoadScene(const std::string &path, Scene *scene)
    {
        
    }

    void Application::LoadEntity(const std::string &dllName, UUID uuid)
    {
        std::string dllPath = ENTITY_DLL_PATH + '/' + dllName;
        void* h = dlopen(dllPath.c_str(), RTLD_NOW);

        if(!h)
        {
            std::cerr << "dlopen: " << dlerror() << '\n';
            assert(false);
        }

        m_CurrentScene.EntityByUUID(uuid);
        m_CurrentScene.AttachComponent<Code>(m_CurrentScene.EntityByUUID(uuid));
        
        using FactorryStart = void(*)();

        auto start = (FactorryStart) dlsym(h, "Start");

        if(!start)
        {
            std::cerr << "factorry returned nullptr\n";
            assert(false);
        }

        m_CurrentScene.GetComponent<Code>(uuid)->Start = start;

        using FactorryUpdate = void(*)(float deltaTime);

        auto update = (FactorryUpdate) dlsym(h, "Update");

        if(!update)
        {
            std::cerr << "factorry returned nullptr\n";
            assert(false);
        }
    }

    Application *CreateApplication()
    {
        return new Application();
    }
}
