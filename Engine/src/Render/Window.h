#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Sauce
{
  class Window
  {
  public:
    Window();
    Window(std::string title, uint32_t width, uint32_t height, std::function<void()> onShouldClose);
    void Init();
    void Update();
    void Destroy();
  private:
    uint32_t m_Width, m_Height;
    std::string m_Title;
    GLFWwindow *m_Window;
    std::function<void()> m_OnShouldClose;
  };
}
