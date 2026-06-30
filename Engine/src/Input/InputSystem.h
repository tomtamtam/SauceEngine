#pragma once

#include "GLFW/glfw3.h"
#include "Input/KeyboardListener.h"
namespace Sauce
{
    class InputSystem
    {
    public:
        bool GetKeyDown(int key);
        bool GetKeyReleased(int key);
        bool GetKeyJustPressed(int key);

        void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    private:
        KeyboardListener m_KeyboardListener;
    };
}
