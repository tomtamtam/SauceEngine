#pragma once

#include "GLFW/glfw3.h"
#include "Input/KeyboardListener.h"
namespace Sauce
{
    class InputSystem
    {
    public:
        InputSystem();
        ~InputSystem();
        bool GetKeyDown(int key);
        bool GetKeyReleased(int key);
        bool GetKeyJustPressed(int key);

        void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
        void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
        void CharCallback(GLFWwindow* window, unsigned int codepoint);
        void EnterCallback(GLFWwindow* window, int entered);
        void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
        void WindowFocusCallback(GLFWwindow* window, int focused);
    private:
        KeyboardListener m_KeyboardListener;
    };
}
