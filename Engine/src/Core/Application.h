#pragma once

#include "Render/Window.h"
#include "Scene/Scene.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <cstdint>

namespace Sauce
{
  class Application : public std::enable_shared_from_this<Application>
  {
  public:

    Application();
    virtual ~Application() = default;

    void Run();
    void Terminate();

  private:
    void Setup();

    bool m_IsRunning;
    std::string m_Name;

    std::shared_ptr<Scene> m_CurrentScene;
    std::unordered_map<uint64_t, std::shared_ptr<Scene>> m_Scenes;

    Window m_CurrentWindow;
  };
}
