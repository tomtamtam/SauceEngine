#include "Window.h"
#include "GLFW/glfw3.h"
#include <cassert>
#include <functional>
#include <iostream>
#include <string>

namespace Sauce {
    void Window::Init() {
        std::cout << "win init\n";
        glfwSetErrorCallback([](int error, const char *desc) {
            std::cerr << "GLFW Error " << error << ": " << desc << std::endl;
        });

        if (!glfwInit()) {
            assert(false && "Error: Failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_DEPTH_BITS, 24);

        m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);

        if (m_Window == NULL)
        {
            glfwTerminate();
            assert(false && "Error: failes to create GLFW Window");
        }
        glfwMakeContextCurrent(m_Window);

        // Self
        glfwSetWindowUserPointer(m_Window, this);

        // Key Callback
        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mod) {
                auto *self = static_cast<Window*>(glfwGetWindowUserPointer(window));
                self->m_OnKeyCallback(window, key, scancode, action, mod);
                });


        // Resize Callback
        glfwSetFramebufferSizeCallback(m_Window,
        [](GLFWwindow* window, int width, int height)
            {
                auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
                self->m_Width = width;
                self->m_Height = height;
                glViewport(0, 0, width, height);
            });

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            glfwTerminate();
            assert(false && "Error: failed to load GLAD");
        }

        glfwSwapInterval(0);

        //blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


        //depth
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glDisable(GL_CULL_FACE);
    }

    void Window::Update()
    {
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
        if (glfwWindowShouldClose(m_Window))
            m_OnShouldClose();
    }

    void Window::Destroy() { glfwTerminate(); }

    Window::Window()
        : m_Title("test"), m_Width(800), m_Height(800), m_Window(nullptr) 
    {
        m_OnShouldClose = []() {};
    }

    Window::Window(std::string title, uint32_t width, uint32_t height, std::function<void()> onShouldClose, std::function<void(GLFWwindow *window, int key, int scancode, int action, int mod)> onKeyCallback)
        : m_Title(title), m_Width(width), m_Height(height), m_Window(nullptr), m_OnShouldClose(onShouldClose), m_OnKeyCallback(onKeyCallback)
    {
    }

    uint32_t Window::GetWidth() const
    {
        return m_Width;
    }

    uint32_t Window::GetHeight() const 
    {
        return m_Height;
    }
}
