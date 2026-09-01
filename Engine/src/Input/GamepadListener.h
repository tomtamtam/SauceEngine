#pragma once

#include "Input/Gamepad.h"
#include <unordered_map>

namespace Sauce
{
    class GamepadListener
    {
    public:
        GamepadListener();
        ~GamepadListener();

        void BeginFrame() const;
        void EndFrame() const;

        void ConnectionCallback(int jid, int event);

        void OnConnect(int jid);
        void OnDisconnect(int jid);

        float GetAxis(int jid, int axis) const;
        bool IsButtonPressed(int jid, int button) const;
        bool IsButtonReleased(int jid, int button) const;
        bool IsButtonJustPressed(int jid, int button) const;

        inline bool IsConnected(int jid) const { return m_Slots[jid]; }

    private:
        bool m_Slots[16];
        std::unordered_map<int, Gamepad*> m_Gamepads;
    };
}
