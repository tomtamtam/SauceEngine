#include "Render/Renderer.h"
#include "GLFW/glfw3.h"
#include "ImGuiLayer.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include <iostream>
#include <memory>

auto renderer = std::make_shared<Sauce::Renderer>();
ImGuiLayer layer;
bool shouldClose = false;

void onClose()
{
    std::cout << "close\n";
    shouldClose = true;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
    if (ImGui::GetIO().WantCaptureKeyboard)
        return;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
}

void char_callback(GLFWwindow* window, unsigned int codepoint)
{
    ImGui_ImplGlfw_CharCallback(window, codepoint);
}

void enter_callback(GLFWwindow* window, int entered)
{
    ImGui_ImplGlfw_CursorEnterCallback(window, entered);
}

void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
    ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
}

void window_focus_callback(GLFWwindow* window, int focused)
{
    ImGui_ImplGlfw_WindowFocusCallback(window, focused);
}

int main()
{
    renderer->CreateWindow("SauceEditor", 800, 800, onClose, key_callback, mouse_button_callback, scroll_callback, char_callback, enter_callback, cursor_pos_callback, window_focus_callback);

    layer.Init(renderer->GetWindow());

    while(!shouldClose)
    {
        glfwPollEvents();
        if (glfwGetWindowAttrib(renderer->GetWindow(), GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }
        layer.BeginFrame();
            ImGui::Begin("Test");
            if(ImGui::Button("Test"))
                std::cout << "pressed\n";
            ImGui::End();
        layer.EndFrame();
        glfwSwapBuffers(renderer->GetWindow());
    }
    layer.Shutdown();
}
