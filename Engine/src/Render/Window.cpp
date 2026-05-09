#include "Window.h"
#include "GLFW/glfw3.h"
#include <cassert>
#include <functional>
#include <string>
#include <iostream>

namespace Sauce
{
  void Window::Init()
  {
    std::cout << "win";
    if(!glfwInit())
      assert("Error: Failed to initialize GLFW");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);

    if (m_Window == NULL)
    {
      glfwTerminate();
      assert("Error: failes to create GLFW Window");
    }
    glfwMakeContextCurrent(m_Window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
      glfwTerminate();
      assert("Error: failed to load GLAD");
    } 
  }

  void Window::Update()
  {
    glfwSwapBuffers(m_Window);
    glfwPollEvents();
    if(glfwWindowShouldClose(m_Window))
      m_OnShouldClose();
  }

  void Window::Destroy()
  {
    glfwTerminate();
  }

  Window::Window()
    : m_Title("test"), m_Width(800), m_Height(800), m_Window(nullptr)
  {
    m_OnShouldClose = [](){};
  }

  Window::Window(std::string title, uint32_t width, uint32_t height, std::function<void()> onShouldClose)
    : m_Title(title), m_Width(width), m_Height(height), m_Window(nullptr), m_OnShouldClose(onShouldClose)
  {
    std::cout << "constructor";
  }
}
