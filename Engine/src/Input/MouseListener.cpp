#include "MouseListener.h"
#include "GLFW/glfw3.h"
#include <cstring>

namespace Sauce
{
    MouseListener::MouseListener()
        : m_X(0), m_Y(0)
    {
        memset(m_Buttons, false, sizeof m_Buttons);
        memset(m_ButtonsLast, false, sizeof m_ButtonsLast);
    }
    MouseListener::~MouseListener()
    {}

    void MouseListener::EndFrame()
    {
        for(int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
        {
            m_ButtonsLast[i] = m_Buttons[i];
        }
        m_ScrollX = 0;
        m_ScrollY = 0;
    }

    bool MouseListener::IsButtonPressed(int button) const
    {
        if(button < 0 || button > GLFW_MOUSE_BUTTON_LAST)
            return false;

        return m_Buttons[button];
    }

    bool MouseListener::IsButtonReleased(int button) const
    {
        if(button < 0 || button > GLFW_MOUSE_BUTTON_LAST)
            return false;

        return m_ButtonsLast[button] && !m_Buttons[button];
    }

    bool MouseListener::IsButtonJustPressed(int button) const
    {
        if(button < 0 || button > GLFW_MOUSE_BUTTON_LAST)
            return false;

        return !m_ButtonsLast[button] &&m_Buttons[button];
    }

    float MouseListener::GetX() const
    {
        return m_X;
    }

    float MouseListener::GetY() const
    {
        return m_Y;
    }

    glm::vec2 MouseListener::GetPos() const
    {
        return {m_X, m_Y};
    }

    float MouseListener::GetScrollX() const
    {
        return m_ScrollX;
    }

    float MouseListener::GetScrollY() const
    {
        return m_ScrollY;
    }

    glm::vec2 MouseListener::GetScrollOffset() const
    {
        return {m_ScrollX, m_ScrollY};
    }

    void MouseListener::ButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        if(button < 0 || button > GLFW_MOUSE_BUTTON_LAST)
            return;

        if(action == GLFW_PRESS)
            m_Buttons[button] = true;

        if(action == GLFW_RELEASE)
            m_Buttons[button] = false;
    }

    void MouseListener::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        m_ScrollX = xoffset;
        m_ScrollY = yoffset;
    }

    void MouseListener::PositionCallback(GLFWwindow* window, double xpos, double ypos)
    {
        m_X = xpos;
        m_Y = ypos;
    }
}
