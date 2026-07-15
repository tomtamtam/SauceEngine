#include "defines.h"
#include "Render/Renderer.h"
#include "GLFW/glfw3.h"
#include "ImGuiLayer.h"
#include "imgui_impl_glfw.h"
#include <memory>

EditorState state {
    std::make_shared<Sauce::Renderer>(),
    ImGuiLayer(),
    false
};

void onClose()
{
    state.shouldClose = true;
}

void Update();

int main()
{
    state.renderer->CreateWindow("SauceEditor", 800, 800, onClose, key_callback, mouse_button_callback, scroll_callback, char_callback, enter_callback, cursor_pos_callback, window_focus_callback);
    state.layer.Init(state.renderer->GetWindow());

    while(!state.shouldClose)
    {
        state.renderer->Clear();
        glfwPollEvents();
        if (glfwGetWindowAttrib(state.renderer->GetWindow(), GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }
        state.layer.BeginFrame();
            Update();
        state.layer.EndFrame();
        glfwSwapBuffers(state.renderer->GetWindow());
    }
    state.layer.Shutdown();
}

void Update()
{

}
