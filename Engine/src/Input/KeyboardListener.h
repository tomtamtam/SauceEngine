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

        void EndFrame();

        bool GetKeyDown(int key) const;
        bool GetKeyReleased(int key) const;
        bool GetKeyJustPressed(int key) const;

    private:
        bool m_Keys[GLFW_KEY_LAST + 1];
        bool m_LastKeys[GLFW_KEY_LAST + 1];
    };
}
