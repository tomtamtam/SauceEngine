# pragma once

#include "GLFW/glfw3.h"

namespace Sauce
{
    class Gamepad
    {
    public:
        Gamepad(int jid);
        ~Gamepad();

        bool IsButtonPressed(int button) const;
        bool IsButtonReleased(int button) const;
        bool IsButtonJustPressed(int button) const;
        float GetAxis(int axis) const;
        inline int GetJid() const { return m_Jid; }

        void BeginFrame();
        void EndFrame();
    private:
        int m_Jid;
        GLFWgamepadstate m_State;
        bool m_Buttons[GLFW_GAMEPAD_BUTTON_LAST + 1];
        bool m_ButtonsLast[GLFW_GAMEPAD_BUTTON_LAST + 1];
    	float m_Axes[GLFW_GAMEPAD_AXIS_LAST + 1];
    };
}
