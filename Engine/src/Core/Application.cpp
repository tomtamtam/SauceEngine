#include "Application.h"
#include "ECS/Components.h"
#include <cstdint>
#include <memory>

namespace Sauce
{
    Application::Application()
        : m_IsRunning(true), m_CurrentScene("Main Scene")
    {
    }


    void Application::Setup()
    {
        uint32_t s_ComponentCounter = 0;
        auto self = shared_from_this();
        auto onShouldClose = [self](){self->Terminate();};
        m_CurrentWindow = {"test", 800, 800, onShouldClose};
        m_CurrentWindow.Init();

        UUID ent = m_CurrentScene.AddEntity();
        m_CurrentScene.AttachComponent<Transform>(ent);
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
