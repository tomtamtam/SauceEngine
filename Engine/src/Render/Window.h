#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdint>
#include <functional>
#include <string>


namespace Sauce
{
    class Window
    {
    public:
        Window();
        Window(std::string title, uint32_t width, uint32_t height, std::function<void()> onShouldClose, std::function<void(GLFWwindow *window, int key, int scancode, int action, int mod)> onKeyCallback);
        void Init();
        void Update();
        void Destroy();
    private:
        uint32_t m_Width, m_Height;
        std::string m_Title;
        GLFWwindow *m_Window;
        std::function<void()> m_OnShouldClose;
        std::function<void(GLFWwindow *window, int key, int scancode, int action, int mod)> m_OnKeyCallback;
    };
}
