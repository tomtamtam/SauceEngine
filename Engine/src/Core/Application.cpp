#include "Application.h"
#include <memory>

namespace Sauce
{
  Application::Application()
  {
    m_IsRunning = true;
    auto self = shared_from_this();
    auto onShouldClose = [self](){self->Terminate();};
    m_CurrentWindow = {"test", 800, 800, onShouldClose};
  }


  void Application::Setup()
  {
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
    }

    m_CurrentWindow.Destroy();
  }
}
