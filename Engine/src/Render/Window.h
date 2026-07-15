#pragma once

#include <GLFW/glfw3.h>
#include <cstdint>
#include <functional>
#include <string>

namespace Sauce
{
    #define SauceKeyCallback std::function<void(GLFWwindow* window, int key, int scancode, int action, int mods)>
    #define SauceMouseButtonCallback std::function<void(GLFWwindow* window, int button, int action, int mods)>
    #define SauceScrollCallback std::function<void(GLFWwindow* window, double xoffset, double yoffset)>
    #define SauceCharCallback std::function<void(GLFWwindow* window, unsigned int codepoint)>
    #define SauceEnterCallback std::function<void(GLFWwindow* window, int entered)>
    #define SauceCurtsorPositionCallback std::function<void(GLFWwindow* window, double xpos, double ypos)>
    #define SauceWindowFocusCallback std::function<void(GLFWwindow* window, int focused)>

    class Window
    {
    public:
        Window();
        Window(std::string title, uint32_t width, uint32_t height, std::function<void()> onShouldClose, SauceKeyCallback keyCallback, SauceMouseButtonCallback mouseButtonCallback, SauceScrollCallback scrollCallback, SauceCharCallback charCallback, SauceEnterCallback enterCallback, SauceCurtsorPositionCallback cursorPositionCallback, SauceWindowFocusCallback windowFocusCallback);
        void Init();
        void Update();
        void Destroy();
        uint32_t GetWidth() const;
        uint32_t GetHeight() const;
        GLFWwindow *GetWindow() const;
    private:
        uint32_t m_Width, m_Height;
        std::string m_Title;
        GLFWwindow *m_Window;
        std::function<void()> m_OnShouldClose;
        SauceKeyCallback m_OnKeyCallback;
        SauceMouseButtonCallback m_MouseButtonCallback;
        SauceScrollCallback m_ScrollCallback;
        SauceCharCallback m_CharCallback;
        SauceEnterCallback m_EnterCallback;
        SauceCurtsorPositionCallback m_CursorPosCallback;
        SauceWindowFocusCallback m_WindowFocusCallback;
    };
}
