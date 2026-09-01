# pragma once

#include "GLFW/glfw3.h"
#include "glm/ext/vector_float2.hpp"

namespace Sauce
{
    class MouseListener
    {
    public:
        MouseListener();
        ~MouseListener();
        void ButtonCallback(GLFWwindow* window, int button, int action, int mods);
        void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
        void PositionCallback(GLFWwindow* window, double xpos, double ypos);

        void EndFrame();

        bool IsButtonPressed(int button) const;
        bool IsButtonReleased(int button) const;
        bool IsButtonJustPressed(int button) const;
        float GetX() const;
        float GetY() const;
        glm::vec2 GetPos() const;
        float GetScrollX() const;
        float GetScrollY() const;
        glm::vec2 GetScrollOffset() const;

    private:
        double m_X, m_Y;
        bool m_Buttons[GLFW_MOUSE_BUTTON_LAST + 1];
        bool m_ButtonsLast[GLFW_MOUSE_BUTTON_LAST + 1];
        double m_ScrollX, m_ScrollY;
    };
}
