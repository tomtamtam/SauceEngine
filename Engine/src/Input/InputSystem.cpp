#include "InputSystem.h"

namespace Sauce
{
    bool InputSystem::GetKeyDown(int key)
    {
        return m_KeyboardListener.GetKeyDown(key);
    }

    bool InputSystem::GetKeyReleased(int key)
    {
        return m_KeyboardListener.GetKeyReleased(key);
    }

    bool InputSystem::GetKeyJustPressed(int key)
    {
        return m_KeyboardListener.GetKeyJustPressed(key);
    }

    void InputSystem::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        m_KeyboardListener.Callback(window, key, scancode, action, mods);
    }

    void InputSystem::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
    }

    void InputSystem::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {}
    void InputSystem::CharCallback(GLFWwindow* window, unsigned int codepoint)
    {}
    void InputSystem::EnterCallback(GLFWwindow* window, int entered)
    {}
    void InputSystem::CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
    {}
    void InputSystem::WindowFocusCallback(GLFWwindow* window, int focused)
    {}

    InputSystem::InputSystem()
    {}

    InputSystem::~InputSystem()
    {}
}
