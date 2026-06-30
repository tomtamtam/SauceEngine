#include "KeyboardListener.h"
#include <cstring>

namespace Sauce
{
    KeyboardListener::KeyboardListener()
    {
        memset(m_Keys, 0, sizeof m_Keys);
        memset(m_LastKeys, 0, sizeof m_LastKeys);
    }

    void KeyboardListener::Callback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        m_LastKeys[key] = m_Keys[key];
        if (key >= 0 && key < GLFW_KEY_LAST)
        {
            if (action == GLFW_PRESS)
            {
                m_Keys[key] = true;
            }
            else if (action == GLFW_RELEASE)
            {
                m_LastKeys[key] = false;
            }
        }
    }

    bool KeyboardListener::GetKeyDown(int key)
    {
        if (key >= 0 && key < GLFW_KEY_LAST)
        {
            return m_Keys[key];
        }
        return false;
    }

    bool KeyboardListener::GetKeyReleased(int key)
    {
        if (key >= 0 && key < GLFW_KEY_LAST)
        {
            return !m_Keys[key] && m_LastKeys[key];
        }
        return false;
    }

    bool KeyboardListener::GetKeyJustPressed(int key)
    {
        if (key >= 0 && key < GLFW_KEY_LAST)
        {
            return m_Keys[key] && !m_LastKeys[key];
        }
        return false;
    }

    KeyboardListener::~KeyboardListener() {}
}
