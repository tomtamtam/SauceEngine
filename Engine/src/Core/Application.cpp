#include "Application.h"
#include "ECS/Components.h"
#include <cstdint>
#include <iostream>
#include <memory>

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

        uint64_t ent = m_CurrentScene.AddEntity();
        m_CurrentScene.AttachComponent<Transform>(ent);
        m_CurrentScene.GetComponent<Transform>(ent)->print();
        m_CurrentScene.RemoveComponent<Transform>(ent);
        m_CurrentScene.AttachComponent<Transform>(ent);

        m_CurrentScene.DestroyEntity(ent);

        ent = m_CurrentScene.AddEntity();
        m_CurrentScene.AttachComponent<Transform>(ent);
        std::cout << "finished app setup\n";
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
}
