#include "../Shared/Shared.h"
#include "ECS/Entity.h"
#include "ECS/Components.h"
#include "defines.h"
#include "GLFW/glfw3.h"
#include "ImGuiLayer.h"
#include "entt/entity/entity.hpp"
#include "entt/entity/fwd.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_internal.h"
#include <GL/glext.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <format>
#include <nfd.h>
#include <string>


EditorState state = {};

void onClose()
{
    state.shouldClose = true;
}

std::string validPath(const std::string &current)
{
    if(current.ends_with('/'))
        return current.substr(0, current.size() - 1);
    return current;
}


class ProjectSelector
{
private:
    enum class SelectionState
    {
        MAIN_SCREEN,
        CREATE_SCREEN,
        OPEN_SCREEN
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

        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);


        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("Select Projct", nullptr, hostFlags);
            if(ImGui::Button("Create Project"))
            {
                m_State = SelectionState::CREATE_SCREEN;
            }
            if(ImGui::Button("Open Folder"))
            {
                m_State = SelectionState::OPEN_SCREEN;
            }
            if(ImGui::Button("Quit"))
            {
                state.shouldClose = true;
            }
        ImGui::End();
        ImGui::PopStyleVar(3);
        switch(m_State)
        {
            case SelectionState::MAIN_SCREEN:
                break;
            case SelectionState::CREATE_SCREEN:
                ImGui::SetNextWindowPos(ImVec2(viewport->WorkSize.x / 2 - 350, viewport->WorkSize.y / 2 - 100));
                ImGui::SetNextWindowSize(ImVec2(700, 200), ImGuiCond_Always);
                ImGui::Begin("Create Project", nullptr, hostFlags);
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
                            std::string finalPath = std::format("{}/{}", validPath(m_ProjPath), m_ProjName);
                            if(std::filesystem::exists(finalPath))
                            {
                                m_AlreadyExsists = true;
                            }
                            else
                            {
                                createProject(m_ProjName, validPath(m_ProjPath));
                                m_State = SelectionState::MAIN_SCREEN;
                                *m_Path = finalPath;
                                shouldReturn = true;
                                m_InvalidPath = false;
                                m_AlreadyExsists = false;
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
                        m_InvalidPath = false;
                        m_AlreadyExsists = false;
                    }
                ImGui::End();
                break;

            case SelectionState::OPEN_SCREEN:
                ImGui::SetNextWindowSize(ImVec2(700, 200), ImGuiCond_Always);
                ImGui::SetNextWindowPos(ImVec2(viewport->WorkSize.x / 2 - 350, viewport->WorkSize.y / 2 - 100));
                ImGui::Begin("Open Project");
                    ImGui::InputText("Path", m_ProjPath, 200);
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
                    if(m_InvalidPath)
                    {
                        ImGui::TextColored({255, 0, 0, 1}, "Path %s isn't a SauceEngine project", m_ProjPath);
                    }
                    if(ImGui::Button("Open"))
                    {
                        std::string finalPath = std::format("{}/.project_info.json", m_ProjPath);
                        if(std::filesystem::exists(finalPath))
                        {
                            shouldReturn = true;
                            *m_Path = validPath(m_ProjPath);
                            m_InvalidPath = false;
                            m_State = SelectionState::MAIN_SCREEN;
                        }
                        else
                        {
                            m_InvalidPath = true;
                        }
                    }
                    ImGui::SameLine();
                    if(ImGui::Button("Cancel"))
                    {
                        m_State = SelectionState::MAIN_SCREEN;
                        m_InvalidPath = false;
                    }
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

class Editor
{
public:
    Editor(std::string *projPath) 
        : m_ProjPath(*projPath),
        viewStates(false, true, true, true, true, true),
        m_DefaultScene(""),
        m_FirstTime(true),
        m_SelectedEntity()
    {
    }
    ~Editor() {}
    bool Update()
    {
        if(m_FirstTime)
        {
            m_ProjPath = state.projPath;
            std::cout << "\x1B[32mEditing\033[0m " << m_ProjPath << '\n';
            load();
            m_FirstTime = false;
        }
        DrawMainWindow();
        DrawRunWindow();
        DrawConsole();
        DrawAssetBrowser();
        DrawSceneHierarchy();
        DrawInspector();
        return true;
    }
private:
    struct ViewStates {bool runWindow, viewport, sceneHierarchy, inspector, console, assetBrowser;};
    std::string m_ProjPath;
    bool m_FirstTime;
    ViewStates viewStates;
    char buffer[100];
    bool m_ShowPopup;
    std::string m_DefaultScene;
    entt::entity m_SelectedEntity;

    void load()
    {
        json j;
        std::ifstream f(validPath(m_ProjPath) + "/.project_info.json");
        std::cout << "json path: " << validPath(m_ProjPath) + "/.project_info.json" << '\n';
        f >> j;
        m_DefaultScene = std::format("{}/{}", validPath(m_ProjPath), j["defaultScene"].get<std::string>());
        state.sceneSerializer.Deserialze(m_DefaultScene);
        std::cout << "Desiralized: " << m_DefaultScene << '\n';
    }

    void DrawMenuBar()
    {
        if(ImGui::BeginMenuBar())
        {
            if(ImGui::BeginMenu("File"))
            {
                if(ImGui::MenuItem("Save", "Ctrl+S"))
                {}
                if(ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
                {}
                if(ImGui::MenuItem("Quit", "Ctrl+Q"))
                {
                    state.shouldClose = true;
                }
                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("Edit"))
            {
                if(ImGui::MenuItem("Undo", "Ctrl+Z"))
                {}
                if(ImGui::MenuItem("Redo", "Ctrl+Shift+Z"))
                {}
                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("Poject"))
            {
                if(ImGui::MenuItem("Run", "F5"))
                {
                    run();
                }
                if(ImGui::MenuItem("Build", "F6"))
                {}
                if(ImGui::MenuItem("Quit to Project List", "Ctrl+SHIFT+Q"))
                {
                    state.inProject = false;
                }
                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("View"))
            {
                    ImGui::MenuItem("Run Window", nullptr, &viewStates.runWindow);
                    ImGui::MenuItem("Viewport", nullptr, &viewStates.viewport);
                    ImGui::MenuItem("Scene Hierarchy", nullptr, &viewStates.sceneHierarchy);
                    ImGui::MenuItem("Inspector", nullptr, &viewStates.inspector);
                    ImGui::MenuItem("Console", nullptr, &viewStates.console);
                    ImGui::MenuItem("Asset-Browser", nullptr, &viewStates.assetBrowser);
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
    }

    void DrawMainWindow()
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("EditorDockSpaceHost", nullptr, hostFlags);
            ImGui::PopStyleVar(3);

            ImGuiID dockspaceId = ImGui::GetID("EditorDockSpace");
            ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
            if(!state.initializedDock)
            {
                state.initializedDock = true;

                ImGui::DockBuilderRemoveNode(dockspaceId);
                ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);
                ImGui::DockBuilderSetNodeSize(dockspaceId, viewport->Size);


                ImGuiID dockMain = dockspaceId;
                ImGuiID dockLeft   = ImGui::DockBuilderSplitNode(dockMain, ImGuiDir_Left, 0.20f, nullptr, &dockMain);
                ImGuiID dockRight  = ImGui::DockBuilderSplitNode(dockMain, ImGuiDir_Right, 0.25f, nullptr, &dockMain);
                ImGuiID dockBottom = ImGui::DockBuilderSplitNode(dockMain, ImGuiDir_Down, 0.28f, nullptr, &dockMain);

                ImGui::DockBuilderDockWindow("Console", dockBottom);
                ImGui::DockBuilderDockWindow("Scene Hierarchy", dockLeft);
                ImGui::DockBuilderDockWindow("Inspector", dockRight);
                ImGui::DockBuilderDockWindow("Viewport", dockMain);
                ImGui::DockBuilderDockWindow("Asset-Browser", dockBottom);

                ImGui::DockBuilderFinish(dockspaceId);
            }
            DrawMenuBar();
        ImGui::End();
    }

    void DrawRunWindow()
    {
        if(!viewStates.runWindow)
            return;
        ImGui::SetNextWindowSize(ImVec2(140, 35), ImGuiCond_Always);
        ImGui::Begin("RunWindow", nullptr, floatingFlags);
            if(ImGui::Button("Run"))
            {
                run();
            }
            ImGui::SameLine();
            if(ImGui::Button("Pause"))
            {}
            ImGui::SameLine();
            if(ImGui::Button("Stop"))
            {}
        ImGui::End();
    }

    void DrawConsole()
    {
        if(!viewStates.console)
            return;
        ImGui::Begin("Console", &viewStates.console);
            if(ImGui::Button("Clear")) state.console.clear();
            ImGui::SameLine();
            if(ImGui::Button("Copy All")) state.console.copyAll();
            ImGui::SameLine();
            ImGui::Checkbox("Auto Scroll", &state.console.autoScroll);
            ImGui::SameLine();
            ImGui::Checkbox("Show Warnings", &state.console.showWarnings);

            ImGui::Separator();
            ImGui::BeginChild("ScrollRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

            for(auto msg : state.console.messages)
            {
                if(msg.type == MessageType::WARNING && !state.console.showWarnings) continue;

                ImVec4 color;
                std::string prefix;
                switch(msg.type)
                {
                    case MessageType::INFO: color = ImVec4(0.0f, 0.6f, 0.6f, 1.0f); prefix = "[Info]"; break;
                    case MessageType::WARNING: color = ImVec4(1.0f, 0.8f, 0.2f, 1.0f); prefix = "[Warning]"; break;
                    case MessageType::ERROR: color = ImVec4(1.0f, 0.35f, 0.35f, 1.0f); prefix = "[ERROR]"; break;
                    default: color = ImVec4(0.6f, 0.2, 0.6f, 1.0f); prefix = "[Debug]"; break;
                }

                ImGui::PushStyleColor(ImGuiCol_Text, color);
                ImGui::TextUnformatted(prefix.c_str());
                ImGui::PopStyleColor();
                ImGui::TextUnformatted(msg.text.c_str());
            }

            if(state.console.autoScroll)
                ImGui::SetScrollHereY(1.0f);

            ImGui::PopStyleVar();
            ImGui::EndChild();
        ImGui::End();
    }

    void DrawAssetBrowser()
    {
        if(!viewStates.assetBrowser)
            return;
        ImGui::Begin("Asset-Browser", &viewStates.assetBrowser);
        ImGui::End();
    }

    void DrawSceneHierarchy()
    {
        if(!viewStates.sceneHierarchy)
            return;

        ImGui::Begin("Scene Hierarchy");
            if(ImGui::Button("Add"))
            {
                m_ShowPopup = true;
                ImGui::OpenPopup("Add Entity");
            }
            ImGui::Separator();
            ImGui::BeginChild("ScrollRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

            auto view = state.scene->m_Registry.view<Sauce::UUIDComponent>();
            for(auto e : view)
            {
                ImGui::PushID((int)e);
                //ImGui::TextUnformatted(state.scene->m_Registry.get<Sauce::UUIDComponent>(e).Name.c_str());
                auto uuid = state.scene->m_Registry.get<Sauce::UUIDComponent>(e);

                bool isSelected = (m_SelectedEntity == e);
                if(ImGui::Selectable(uuid.Name.c_str(), isSelected))
                {
                    m_SelectedEntity = e;
                }
                ImGui::PopID();
            }
            ImGui::PopStyleVar();
            ImGui::EndChild();

            if(m_ShowPopup)
            {
                ImGui::BeginPopup("Add Entity");
                    ImGui::InputText("Name", buffer, 100);
                    if(ImGui::Button("Add"))
                    {
                        std::string name = std::string(buffer);
                        Sauce::Entity e = state.scene->CreateEntity(name);
                        m_ShowPopup = false;
                    }
                    ImGui::SameLine();
                    if(ImGui::Button("Cancel"))
                    {
                        m_ShowPopup = false;
                    }
                ImGui::EndPopup();
            }
        ImGui::End();
    }

    void DrawInspector()
    {
        if(!viewStates.inspector)
            return;
        ImGui::Begin("Inspector");
            if(state.scene->m_Registry.try_get<Sauce::UUIDComponent>(m_SelectedEntity))
            {
                auto uuid = state.scene->m_Registry.get<Sauce::UUIDComponent>(m_SelectedEntity);
                ImGui::Text("Name: %s", uuid.Name.c_str());
                ImGui::Text("UUID: %s", std::to_string((uint64_t)uuid.Id).c_str());
                ImGui::Separator();
                ImGui::Text("Components:");
                ImGui::SameLine();
                if(ImGui::Button("Add"))
                {}
            }
        ImGui::End();
    }

    void run()
    {
        state.console.put(MessageType::INFO, "Run Scene: " + state.scene->m_Name);
    }

    void loadScene(const std::string &scenePath)
    {
        state.sceneSerializer.Deserialze(m_ProjPath + scenePath);
    }
};

void Update();

ProjectSelector projectSelector = {&state.projPath};
Editor editor = {&state.projPath};

int main(int argc, char **argv)
{
    state.renderer->CreateWindow("SauceEditor", 800, 800, onClose, key_callback, mouse_button_callback, scroll_callback, char_callback, enter_callback, cursor_pos_callback, window_focus_callback);
    state.layer.Init(state.renderer->GetWindow());

    if(argc == 2)
    {
        std::string p = argv[1];
        if(std::filesystem::exists(p + "/.project_info.json"))
        {
            state.inProject = true;
            state.projPath = argv[1];
        }
    }
    while(!state.shouldClose)
    {
        state.shouldClose = glfwWindowShouldClose(state.renderer->GetWindow());
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
    if(!editor.Update())
    {
        state.inProject = false;
    }
}
