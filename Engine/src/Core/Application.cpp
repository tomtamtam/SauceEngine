#include "Application.h"
#include <memory>

namespace Sauce
{
  Application::Application()
      : m_IsRunning(true)
  {
    m_CurrentScene = std::make_shared(TestScene);
  }


  void Application::Setup()
  {
    auto self = shared_from_this();
    auto onShouldClose = [self](){self->Terminate();};
    m_CurrentWindow = {"test", 800, 800, onShouldClose};
    m_CurrentWindow.Init();
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
      m_CurrentScene->Update(0.0f);
    }

    m_CurrentWindow.Destroy();
  }
}
