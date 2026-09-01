#pragma once

#include "GLFW/glfw3.h"
#include "Input/GamepadListener.h"
#include "Input/KeyboardListener.h"
#include "Input/MouseListener.h"
#include "glm/ext/vector_float2.hpp"

namespace Sauce
{
    class InputSystem
    {
    public:
        InputSystem();
        ~InputSystem();
        bool GetKeyDown(int key) const;
        bool GetKeyReleased(int key) const;
        bool GetKeyJustPressed(int key) const;

        bool GetMouseButtonDown(int button) const;
        bool GetMouseButtonReleased(int button) const;
        bool GetMouseButtonJustPressed(int button) const;
        float GetMouseX() const;
        float GetMouseY() const;
        glm::vec2 GetMousePos() const;
        float GetMouseScrollX() const;
        float GetMouseScrollY() const;
        glm::vec2 GetMouseScrollOffset() const;

        float GetGamepadAxis(int jid, int axis) const;
        bool GetGamepadButtonPressed(int jid, int button) const;
        bool GetGamepadButtonReleased(int jid, int button) const;
        bool GetGamepadButtonJustPressed(int jid, int button) const;

        bool IsGamepadConnected(int jid) const;

        void BeginFrame();
        void Endframe();

        void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
        void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
        void CharCallback(GLFWwindow* window, unsigned int codepoint);
        void EnterCallback(GLFWwindow* window, int entered);
        void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
        void WindowFocusCallback(GLFWwindow* window, int focused);
    private:
        KeyboardListener m_KeyboardListener;
        MouseListener m_MouseListener;
        GamepadListener m_GamepadListener;
    };
}
