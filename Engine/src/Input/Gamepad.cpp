#include "Gamepad.h"
#include "GLFW/glfw3.h"
#include <cstring>

namespace Sauce
{
    Gamepad::Gamepad(int jid)
        : m_Jid(jid)
    {
        memset(m_Buttons, false, sizeof m_Buttons);
        memset(m_ButtonsLast, false, sizeof m_ButtonsLast);
        memset(m_Axes, 0.0f, sizeof m_Axes);
    }

    Gamepad::~Gamepad()
    {}

    void Gamepad::BeginFrame()
    {
        if (glfwGetGamepadState(m_Jid, &m_State))
    	{
    		for (int i = 0; i < GLFW_GAMEPAD_BUTTON_LAST; i++)
    		{
    			m_Buttons[i] = m_State.buttons[i] == GLFW_PRESS;
    		}
    		for (int i = 0; i < GLFW_GAMEPAD_AXIS_LAST; i++)
    		{
    			m_Axes[i] = m_State.axes[i];
    		}
    	}
    }

    void Gamepad::EndFrame()
    {
        for(int i = 0; i < GLFW_GAMEPAD_BUTTON_LAST; i++)
        {
            m_ButtonsLast[i] = m_Buttons[i];
        }
    }

    bool Gamepad::IsButtonPressed(int button) const
    {
        if(button < 0 || button > GLFW_GAMEPAD_BUTTON_LAST)
            return false;

        return m_Buttons[button];
    }

    bool Gamepad::IsButtonReleased(int button) const
    {
        if(button < 0 || button > GLFW_GAMEPAD_BUTTON_LAST)
            return false;

        return m_ButtonsLast[button] && !m_Buttons[button];
    }

    bool Gamepad::IsButtonJustPressed(int button) const
    {
        if(button < 0 || button > GLFW_GAMEPAD_BUTTON_LAST)
            return false;

        return !m_ButtonsLast[button] && m_Buttons[button];
    }

    float Gamepad::GetAxis(int axis) const
    {
        if(axis < 0 || axis > GLFW_GAMEPAD_AXIS_LAST)
            return 0.0f;

        return m_Axes[axis];
    }
}
