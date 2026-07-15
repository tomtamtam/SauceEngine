#pragma once

#include "Render/Renderer.h"
#include "Editor/UI/ImGuiLayer.h"
#include <memory>

struct EditorState
{
    std::shared_ptr<Sauce::Renderer> renderer;
    ImGuiLayer layer;
    bool shouldClose = false;
    bool inProject;
    std::string projPath;
};


inline void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
    if (ImGui::GetIO().WantCaptureKeyboard)
        return;
}

inline void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
}

inline void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
}

inline void char_callback(GLFWwindow* window, unsigned int codepoint)
{
    ImGui_ImplGlfw_CharCallback(window, codepoint);
}

inline void enter_callback(GLFWwindow* window, int entered)
{
    ImGui_ImplGlfw_CursorEnterCallback(window, entered);
}

inline void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
    ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
}

inline void window_focus_callback(GLFWwindow* window, int focused)
{
    ImGui_ImplGlfw_WindowFocusCallback(window, focused);
}
