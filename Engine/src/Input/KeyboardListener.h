#pragma once

#include "GLFW/glfw3.h"
namespace Sauce
{
    class KeyboardListener
    {
    public:
        KeyboardListener();
        ~KeyboardListener();

        void Callback(GLFWwindow *window, int key, int scancode, int action, int mods);

        bool GetKeyDown(int key);
        bool GetKeyReleased(int key);
        bool GetKeyJustPressed(int key);

    private:
        bool m_Keys[GLFW_KEY_LAST + 1];
        bool m_LastKeys[GLFW_KEY_LAST + 1];
    };
}
