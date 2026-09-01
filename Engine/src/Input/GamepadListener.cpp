#include "GamepadListener.h"
#include "GLFW/glfw3.h"
#include "Input/Gamepad.h"
#include <cstring>

namespace Sauce
{
    GamepadListener::GamepadListener()
    {
        memset(m_Slots, false, sizeof m_Slots);
        for(int i = 0; i < 16; i++)
            m_Gamepads[i] = nullptr;
    }

    GamepadListener::~GamepadListener()
    {}

    void GamepadListener::BeginFrame() const
    {
        for(int i = 0; i < 16; i++)
        {
            if(m_Slots[i])
                m_Gamepads.at(i)->BeginFrame();
        }
    }
    void GamepadListener::EndFrame() const
    {
        for(int i = 0; i < 16; i++)
        {
            if(m_Slots[i])
                m_Gamepads.at(i)->EndFrame();
        }
    }

    void GamepadListener::ConnectionCallback(int jid, int event)
    {
        if(jid < 0 || jid > 15)
            return;

        if(event == GLFW_CONNECTED)
            OnConnect(jid);
        else if(event == GLFW_DISCONNECTED)
            OnDisconnect(jid);
    }

    void GamepadListener::OnConnect(int jid)
    {
        m_Slots[jid] = true;
        m_Gamepads[jid] = new Gamepad(jid);
    }
    void GamepadListener::OnDisconnect(int jid)
    {
        m_Slots[jid] = false;
        m_Gamepads[jid] = nullptr;
    }

    float GamepadListener::GetAxis(int jid, int axis) const
    {
        if(jid < 0 || jid > 15)
            return 0;

        if(!m_Slots[jid])
            return 0;

        return m_Gamepads.at(jid)->GetAxis(axis);
    }
    bool GamepadListener::IsButtonPressed(int jid, int button) const
    {
        if(jid < 0 || jid > 15)
            return 0;

        if(!m_Slots[jid])
            return 0;

        return m_Gamepads.at(jid)->IsButtonPressed(button);
    }
    bool GamepadListener::IsButtonReleased(int jid, int button) const
    {
        if(jid < 0 || jid > 15)
            return 0;

        if(!m_Slots[jid])
            return 0;

        return m_Gamepads.at(jid)->IsButtonReleased(button);
    }
    bool GamepadListener::IsButtonJustPressed(int jid, int button) const
    {
        if(jid < 0 || jid > 15)
            return 0;

        if(!m_Slots[jid])
            return 0;

        return m_Gamepads.at(jid)->IsButtonJustPressed(button);
    }

}
