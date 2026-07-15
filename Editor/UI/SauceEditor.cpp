#include "../Shared/Shared.h"
#include "defines.h"
#include "Render/Renderer.h"
#include "GLFW/glfw3.h"
#include "ImGuiLayer.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include <filesystem>
#include <memory>
#include <format>
#include <nfd.h>

EditorState state {
    std::make_shared<Sauce::Renderer>(),
    ImGuiLayer(),
    false,
    false
};

void onClose()
{
    state.shouldClose = true;
}

class ProjectSelector
{
private:
    enum class SelectionState
    {
        MAIN_SCREEN,
        CREATE_SCREEN,
        OPEN_SCENE
    };
public:
    ProjectSelector(std::string *path)
        : m_Path(path), m_State(SelectionState::MAIN_SCREEN), m_ProjName(0), m_ProjPath(0), m_InvalidPath(false), m_AlreadyExsists(false), m_NfdPath(nullptr)
    {
    }
    ~ProjectSelector()
    {
        delete [] m_NfdPath;
    }

    bool Update()
    {
        bool shouldReturn = false;
        auto viewport = ImGui::GetMainViewport();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(viewport->WorkSize, ImGuiCond_Always);
        ImGui::Begin("Select Projct");
            if(ImGui::Button("Create Project"))
            {
                m_State = SelectionState::OPEN_SCENE;
            }
            if(ImGui::Button("Open Folder"))
            {
                m_State = SelectionState::CREATE_SCREEN;
            }
            if(ImGui::Button("Quit"))
            {
                state.shouldClose = true;
            }
        ImGui::End();
        switch(m_State)
        {
            case SelectionState::MAIN_SCREEN:
                break;
            case SelectionState::CREATE_SCREEN:
                ImGui::SetNextWindowPos(ImVec2(viewport->WorkSize.x / 2 - 350, viewport->WorkSize.y / 2 - 100));
                ImGui::SetNextWindowSize(ImVec2(700, 200), ImGuiCond_Always);
                ImGui::Begin("Create Project");
                    ImGui::InputText("Name", m_ProjName, 100);
                    if(!m_InvalidPath && m_AlreadyExsists)
                    {
                        ImGui::TextColored({255, 0, 0, 1}, "Folder %s already exsist in %s", m_ProjName, m_ProjPath);
                    }
                    ImGui::InputText("Path", m_ProjPath, 200);
                    if(m_InvalidPath)
                    {
                        ImGui::TextColored({255, 0, 0, 1}, "Invalid Path");
                    }
                    ImGui::SameLine();
                    if(ImGui::Button("Browse"))
                    {
                        nfdresult_t res = NFD_PickFolder("~/", &m_NfdPath);
                        if(res == NFD_OKAY)
                        {
                            snprintf(m_ProjPath, sizeof(m_ProjPath), "%s", m_NfdPath);
                            free(m_NfdPath);
                            m_NfdPath = nullptr;
                        }
                    }
                    if(ImGui::Button("Create"))
                    {
                        if(std::filesystem::exists(m_ProjPath))
                        {
                            std::string finalPath = std::format("{}/{}", m_ProjPath, m_ProjName);
                            if(std::filesystem::exists(finalPath))
                            {
                                m_AlreadyExsists = true;
                            }
                            else
                            {
                                createProject(m_ProjName, m_ProjPath);
                                *m_Path = finalPath;
                                shouldReturn = true;
                            }
                        }
                        else {
                            m_InvalidPath = true;
                        }
                    }
                    ImGui::SameLine();
                    if(ImGui::Button("Cancel"))
                    {
                        m_State = SelectionState::MAIN_SCREEN;
                    }
                ImGui::End();
                break;

            case SelectionState::OPEN_SCENE:
                ImGui::SetNextWindowSize(ImVec2(700, 200), ImGuiCond_Always);
                ImGui::SetNextWindowPos(ImVec2(viewport->WorkSize.x / 2 - 350, viewport->WorkSize.y / 2 - 100));
                ImGui::Begin("Open Project");
                ImGui::End();
                break;
        }
        return shouldReturn;
    }
private:
    SelectionState m_State;
    std::string *m_Path;
    char m_ProjName[100];
    char m_ProjPath[200];
    bool m_InvalidPath;
    bool m_AlreadyExsists;
    nfdchar_t *m_NfdPath;
};

void Update();

ProjectSelector projectSelector = {&state.projPath};

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
    if(!state.inProject)
    {
        if(projectSelector.Update())
        {
            state.inProject = true;
        }
        return;
    }
}
