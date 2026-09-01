#include "InputSystem.h"
#include "Input/GamepadListener.h"

namespace Sauce
{
#pragma region Keyboard
    bool InputSystem::GetKeyDown(int key) const
    {
        return m_KeyboardListener.GetKeyDown(key);
    }

    bool InputSystem::GetKeyReleased(int key) const
    {
        return m_KeyboardListener.GetKeyReleased(key);
    }

    bool InputSystem::GetKeyJustPressed(int key) const
    {
        return m_KeyboardListener.GetKeyJustPressed(key);
    }
#pragma endregion

#pragma region Mouse
    bool InputSystem::GetMouseButtonDown(int button) const
    {
        return m_MouseListener.IsButtonPressed(button);
    }

    bool InputSystem::GetMouseButtonReleased(int button) const
    {
        return m_MouseListener.IsButtonReleased(button);
    }

    bool InputSystem::GetMouseButtonJustPressed(int button) const
    {
        return m_MouseListener.IsButtonJustPressed(button);
    }

    float InputSystem::GetMouseX() const
    {
        return m_MouseListener.GetX();
    }

    float InputSystem::GetMouseY() const
    {
        return m_MouseListener.GetY();
    }

    glm::vec2 InputSystem::GetMousePos() const
    {
        return m_MouseListener.GetPos();
    }

    float InputSystem::GetMouseScrollX() const
    {
        return m_MouseListener.GetScrollX();
    }
    float InputSystem::GetMouseScrollY() const
    {
        return m_MouseListener.GetScrollY();
    }
    glm::vec2 InputSystem::GetMouseScrollOffset() const
    {
        return m_MouseListener.GetScrollOffset();
    }
#pragma endregion

#pragma region Gamepad
    float InputSystem::GetGamepadAxis(int jid, int axis) const
    {
        return m_GamepadListener.GetAxis(jid, axis);
    }

    bool InputSystem::GetGamepadButtonPressed(int jid, int button) const
    {
        return m_GamepadListener.IsButtonPressed(jid, button);
    }
    bool InputSystem::GetGamepadButtonReleased(int jid, int button) const
    {
        return m_GamepadListener.IsButtonReleased(jid, button);
    }
    bool InputSystem::GetGamepadButtonJustPressed(int jid, int button) const
    {
        return m_GamepadListener.IsButtonJustPressed(jid, button);
    }

    bool InputSystem::IsGamepadConnected(int jid) const
    {
        return m_GamepadListener.IsConnected(jid);
    }
#pragma endregion

    void InputSystem::BeginFrame()
    {
        m_GamepadListener.BeginFrame();
    }

    void InputSystem::Endframe()
    {
        m_KeyboardListener.EndFrame();
        m_MouseListener.EndFrame();
        m_GamepadListener.EndFrame();
    }

#pragma region Callbacks
    void InputSystem::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        m_KeyboardListener.Callback(window, key, scancode, action, mods);
    }

    void InputSystem::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        m_MouseListener.ButtonCallback(window, button, action, mods);
    }

    void InputSystem::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        m_MouseListener.ScrollCallback(window, xoffset, yoffset);
    }

    void InputSystem::CharCallback(GLFWwindow* window, unsigned int codepoint)
    {}
    void InputSystem::EnterCallback(GLFWwindow* window, int entered)
    {}

    void InputSystem::CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
    {
        m_MouseListener.PositionCallback(window, xpos, ypos);
    }

    void InputSystem::WindowFocusCallback(GLFWwindow* window, int focused)
    {}
#pragma endregion

    InputSystem::InputSystem()
    {}

    InputSystem::~InputSystem()
    {}
}
