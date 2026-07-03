#include "KeyboardListener.h"
#include <cstring>

namespace Sauce
{
    KeyboardListener::KeyboardListener()
    {
        std::memset(m_Keys, false, sizeof(m_Keys));
        std::memset(m_LastKeys, false, sizeof(m_LastKeys));
    }

    KeyboardListener::~KeyboardListener()
    {
    }

    void KeyboardListener::Callback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        if (key < 0 || key > GLFW_KEY_LAST)
            return;

        if (action == GLFW_PRESS)
        {
            m_Keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            m_Keys[key] = false;
        }
    }

    bool KeyboardListener::GetKeyDown(int key)
    {
        if (key < 0 || key > GLFW_KEY_LAST)
            return false;

        return m_Keys[key];
    }

    bool KeyboardListener::GetKeyReleased(int key)
    {
        if (key < 0 || key > GLFW_KEY_LAST)
            return false;

        return !m_Keys[key];
    }

    bool KeyboardListener::GetKeyJustPressed(int key)
    {
        if (key < 0 || key > GLFW_KEY_LAST)
            return false;

        bool justPressed = m_Keys[key] && !m_LastKeys[key];
        m_LastKeys[key] = m_Keys[key];

        return justPressed;
    }
}
