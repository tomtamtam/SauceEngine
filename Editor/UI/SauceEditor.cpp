#include "Core/SceneSerializer.h"
#include "ECS/Components.h"
#include "ECS/Scene.h"
#include "Editor/Shared/Shared.h"
#include "Editor/UI/ImGuiLayer.h"
#include "GLFW/glfw3.h"
#include "Render/Buffers/VertexBuffer.h"
#include "Render/Framebuffer.h"
#include "Render/Mesh.h"
#include "Render/Renderer.h"
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <immintrin.h>
#include <memory>
#include <stdexcept>
#include <string>
#include "Render/Shader.h"
#include "defines.h"
#include "entt/entity/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "nfd.h"

class Editor
{
public:
    Editor(const std::string &projPath)
        : m_Renderer(std::make_shared<Sauce::Renderer>()),
        m_Scene(std::make_shared<Sauce::Scene>(m_Renderer)),
        m_SceneSerializer(m_Scene),
        m_ProjectPath(projPath),
        m_FrameBuffer()
    {}
    ~Editor()
    {}

    void Run()
    {
        if(m_ProjectPath.ends_with("/"))
            m_ProjectPath.resize(m_ProjectPath.length() - 1);

        json j;
        std::ifstream f(std::format("{}/.project_info.json", m_ProjectPath));
        f >> j;

        m_ProjectName = j["name"];
        m_CurrentScenePath = j["defaultScene"];
        if(!std::filesystem::exists(m_ProjectPath + m_CurrentScenePath))
            std::runtime_error(std::format("path to shader: {}/{} is no valid path\n", m_ProjectPath, m_CurrentScenePath).c_str());
        std::string shaderPath = j["defaultShader"];
        m_Renderer->CreateWindow(m_ProjectName, 800, 800, on_close,key_callback, mouse_button_callback, scroll_callback, char_callback, enter_callback, cursor_pos_callback, window_focus_callback);
        m_Layer.Init(m_Renderer->GetWindow());

        m_CurrentShader = std::make_shared<Sauce::Shader>(m_ProjectPath + '/' + shaderPath);
        m_Renderer->SetMainShader(m_CurrentShader);
        m_FrameBuffer = std::make_shared<Sauce::FrameBuffer>(200, 200);

        m_State =
        {
            .initializedDock = false, .shouldClose = false,
            .runState = RunState::STOPPED
        };

        m_ViewStates = {.runWindow = true, .viewport = true, .sceneHierarchy = true, .inspector = true, .console = true, .assetBrowser = true};
        m_EditorCam = {{0.0f, 1.0f, 0.0f}, 60.0f, 200, 200, true, false};
        m_EditorTransform.Translation = {0.0f, 0.0f, 3.0f};
        m_EditorTransform.Rotation = {0.0f, 0.0f, -1.0f};

        std::printf("%s %s %s\n", Escapes::ColoredText("Editing", Escapes::GREEN).c_str(), m_ProjectName.c_str(), Escapes::ColoredTextLight(std::format("({})", m_ProjectPath), Escapes::CYAN).c_str());

        m_SceneSerializer.Deserialze(m_ProjectPath + '/' + m_CurrentScenePath);
        m_Console.put(MessageType::INFO, std::format("Loaded Scene '{}'", m_CurrentScenePath));

        m_BeginTime = glfwGetTime();
        m_EndTime = glfwGetTime();
        m_DT = -1.0f;
        while(!m_State.shouldClose)
        {
            if(m_DT >= 0.0f)
            {
                m_State.shouldClose = glfwWindowShouldClose(m_Renderer->GetWindow());
                m_Renderer->Clear();

                glfwPollEvents();

                if (glfwGetWindowAttrib(m_Renderer->GetWindow(), GLFW_ICONIFIED) != 0)
                {
                    ImGui_ImplGlfw_Sleep(10);
                    continue;
                }

                m_Layer.BeginFrame();
                    Update();
                m_Layer.EndFrame();

                glfwSwapBuffers(m_Renderer->GetWindow());
            }
            m_EndTime = glfwGetTime();
            m_DT = m_EndTime - m_BeginTime;
            m_BeginTime = m_EndTime;
        }
        m_Layer.Shutdown();
        m_Renderer->KillWindow();
    }
private:
    enum class RunState
    {
        STOPPED, RUNNING, PAUSED
    };

    struct EditorState
    {
        bool initializedDock, shouldClose;
        RunState runState;
    };

    struct ViewStates
    {
        bool runWindow, viewport, sceneHierarchy, inspector, console, assetBrowser;
    };

    std::shared_ptr<Sauce::Renderer> m_Renderer;
    std::shared_ptr<Sauce::Scene> m_Scene;
    Sauce::SceneSerializer m_SceneSerializer;
    std::shared_ptr<Sauce::FrameBuffer> m_FrameBuffer;
    entt::entity m_SelectedEntity;

    std::string m_ProjectName;
    std::string m_ProjectPath;

    std::string m_CurrentScenePath;
    std::shared_ptr<Sauce::Shader>  m_CurrentShader;

    float m_BeginTime;
    float m_EndTime;
    float m_DT;

    char m_TextBuffer[256];

    EditorState m_State;
    ViewStates m_ViewStates;

    ImGuiLayer m_Layer;

    Console m_Console;

    Sauce::CameraComponent m_EditorCam;
    Sauce::TransformComponent m_EditorTransform;

    void Update()
    {
        DrawMainWindow();
        DrawRunWindow();
        DrawConsole();
        DrawAssetBrowser();
        DrawSceneHierarchy();
        DrawInspector();
        DrawViewport();
    }

    void RunScene()
    {
        if(m_State.runState == RunState::RUNNING)
            return;

        if(m_State.runState == RunState::PAUSED)
        {
            m_Console.put(MessageType::INFO, std::format("Continue Scene '{}'", m_CurrentScenePath));
            m_State.runState = RunState::RUNNING;
            return;
        }
        m_Console.put(MessageType::INFO, std::format("Run Scene '{}'", m_CurrentScenePath));
        m_State.runState = RunState::RUNNING;
    }

    void PauseScene()
    {
        if(m_State.runState != RunState::RUNNING)
            return;
        m_Console.put(MessageType::INFO, std::format("Paused Scene '{}'", m_CurrentScenePath));
        m_State.runState = RunState::PAUSED;
    }

    void StopScene()
    {
        if(m_State.runState == RunState::STOPPED)
            return;
        m_Console.put(MessageType::INFO, std::format("Stopped Scene '{}'", m_CurrentScenePath));
        m_State.runState = RunState::STOPPED;
    }

    void DrawMenuBar()
    {
        if(ImGui::BeginMenuBar())
        {
            if(ImGui::BeginMenu("File"))
            {
                if(ImGui::MenuItem("Save", "Ctrl+S"))
                {
                    m_SceneSerializer.Serialze(m_ProjectPath + '/' + m_CurrentScenePath);
                    m_Console.put(MessageType::INFO, std::format("Saved Scene ({})", m_CurrentScenePath));
                }
                if(ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
                {}
                if(ImGui::MenuItem("Quit", "Ctrl+Q"))
                {
                    m_State.shouldClose = true;
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
                    RunScene();
                }
                if(ImGui::MenuItem("Build", "F6"))
                {}
                if(ImGui::MenuItem("Quit to Project List", "Ctrl+SHIFT+Q"))
                {

                }
                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("View"))
            {
                    ImGui::MenuItem("Run Window", nullptr, &m_ViewStates.runWindow);
                    ImGui::MenuItem("Viewport", nullptr, &m_ViewStates.viewport);
                    ImGui::MenuItem("Scene Hierarchy", nullptr, &m_ViewStates.sceneHierarchy);
                    ImGui::MenuItem("Inspector", nullptr, &m_ViewStates.inspector);
                    ImGui::MenuItem("Console", nullptr, &m_ViewStates.console);
                    ImGui::MenuItem("Asset-Browser", nullptr, &m_ViewStates.assetBrowser);
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
            if(!m_State.initializedDock)
            {
                m_State.initializedDock = true;

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
        if(!m_ViewStates.runWindow)
            return;
        ImGui::SetNextWindowSize(ImVec2(140, 35), ImGuiCond_Always);
        ImGui::Begin("RunWindow", nullptr, floatingFlags);
            if(ImGui::Button("Run"))
            {
                RunScene();
            }
            ImGui::SameLine();
            if(ImGui::Button("Pause"))
            {
                PauseScene();
            }
            ImGui::SameLine();
            if(ImGui::Button("Stop"))
            {
                StopScene();
            }
        ImGui::End();
    }

    void DrawConsole()
    {
        if(!m_ViewStates.console)
            return;
        ImGui::Begin("Console", &m_ViewStates.console);
            if(ImGui::Button("Clear")) m_Console.clear();
            ImGui::SameLine();
            if(ImGui::Button("Copy All")) m_Console.copyAll();
            ImGui::SameLine();
            ImGui::Checkbox("Auto Scroll", &m_Console.autoScroll);
            ImGui::SameLine();
            ImGui::Checkbox("Show Warnings", &m_Console.showWarnings);

            ImGui::Separator();
            ImGui::BeginChild("ScrollRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

            for(auto msg : m_Console.messages)
            {
                if(msg.type == MessageType::WARNING && !m_Console.showWarnings) continue;

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

            if(m_Console.autoScroll)
                ImGui::SetScrollHereY(1.0f);

            ImGui::PopStyleVar();
            ImGui::EndChild();
        ImGui::End();
    }

    void DrawAssetBrowser()
    {
        if(!m_ViewStates.assetBrowser)
            return;
        ImGui::Begin("Asset-Browser", &m_ViewStates.assetBrowser);
        ImGui::End();
    }

    std::string dupeVersionName(const std::string &name)
    {
        std::string temp = name;
        char version = temp.at(temp.size() - 2);
        if (version == '9')
        {
            temp.at(temp.size() - 2) = '0';
        }
        else
        {
            temp.at(temp.size() - 2) = version + 1;
        }
        return temp;
    }

    template<typename T>
    bool THasComponent(entt::entity e)
    {
        return m_Scene->m_Registry.try_get<T>(e);
    }

    template<typename T>
    void DuplicateComponent(entt::entity e1, entt::entity e2)
    {
        if(m_Scene->m_Registry.try_get<T>(e1))
        {
            T dupe = m_Scene->m_Registry.get<T>(e1);
            m_Scene->m_Registry.emplace_or_replace<T>(e2);
            m_Scene->m_Registry.get<T>(e2) = dupe;
        }
        else
        {
            m_Console.put(MessageType::ERROR, "Component to dupe does not exist");
        }
    }

    void DrawSceneHierarchy()
    {
        if(!m_ViewStates.sceneHierarchy)
            return;

        ImGui::Begin("Scene Hierarchy", &m_ViewStates.sceneHierarchy);
            if(ImGui::Button("Add"))
            {
                ImGui::OpenPopup("Add Entity");
            }
            ImGui::Separator();
            ImGui::BeginChild("ScrollRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

            auto view = m_Scene->m_Registry.view<Sauce::UUIDComponent>();
            for(auto e : view)
            {
                ImGui::PushID((int)e);
                //ImGui::TextUnformatted(state.scene->m_Registry.get<Sauce::UUIDComponent>(e).Name.c_str());
                auto uuid = m_Scene->m_Registry.get<Sauce::UUIDComponent>(e);

                bool isSelected = (m_SelectedEntity == e);
                if(ImGui::Selectable(uuid.Name.c_str(), isSelected))
                {
                    m_SelectedEntity = e;
                }
                if (ImGui::BeginPopupContextItem())
                {
                    // Optional: also select the entity on right-click so it's clear what you're acting on
                    m_SelectedEntity = e;
                    if (ImGui::MenuItem("Rename"))
                    {
                        ImGui::OpenPopup("Rename Object");
                    }
                    if (ImGui::MenuItem("Duplicate"))
                    {
                        std::string newName = m_Scene->m_Registry.get<Sauce::UUIDComponent>(m_SelectedEntity).Name;
                        if(newName.ends_with(')'))
                        {
                            newName = dupeVersionName(newName);
                        }
                        else
                        {
                            newName.push_back('(');
                            newName.push_back('1');
                            newName.push_back(')');
                        }
                        auto e = m_Scene->m_Registry.create();
                        m_Scene->m_Registry.emplace_or_replace<Sauce::UUIDComponent>(e, newName);

                        if(THasComponent<Sauce::TransformComponent>(m_SelectedEntity))
                        {
                            DuplicateComponent<Sauce::TransformComponent>(m_SelectedEntity, e);
                        }

                        if(THasComponent<Sauce::CameraComponent>(m_SelectedEntity))
                        {
                            DuplicateComponent<Sauce::CameraComponent>(m_SelectedEntity, e);
                        }

                        if(THasComponent<Sauce::Script>(m_SelectedEntity))
                        {
                            DuplicateComponent<Sauce::Script>(m_SelectedEntity, e);
                        }

                        if(THasComponent<Sauce::MeshInstance>(m_SelectedEntity))
                        {
                            DuplicateComponent<Sauce::MeshInstance>(m_SelectedEntity, e);
                        }
                    }
                    if (ImGui::MenuItem("Delete"))
                    {
                        m_Scene->m_Registry.destroy(m_SelectedEntity);
                    }
                    ImGui::EndPopup();
                }
                if(ImGui::BeginPopup("Rename Object"))
                {
                    char nameBuffer[128];
                    ImGui::InputText("Name", nameBuffer, 128);
                    if(ImGui::Button("Ok"))
                    {
                        std::snprintf(nameBuffer, sizeof(nameBuffer), "%s", m_Scene->m_Registry.get<Sauce::UUIDComponent>(m_SelectedEntity).Name.c_str());
                    }
                    if(ImGui::Button("Cancel"))
                    {
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }
                ImGui::PopID();
            }
            ImGui::PopStyleVar();
            ImGui::EndChild();


            if(ImGui::BeginPopup("Add Entity"))
            {
                ImGui::InputText("Name", m_TextBuffer, 100);
                if(ImGui::Button("Add"))
                {
                    std::string name = { m_TextBuffer };
                    Sauce::Entity e = m_Scene->CreateEntity(name);
                    m_Console.put(MessageType::INFO, std::format("Added Object '{}'", name));
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine();
                if(ImGui::Button("Cancel"))
                {
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        ImGui::End();
    }

    const std::unordered_map<int, std::string> COMPONENT_MAP =
    {
        {TRANSFORM_CIDX, "Transform"},
        {CAMERA_CIDX, "Camera"},
        {MESH_INSTANCE_CIDX, "Mesh Instance"},
        {SCRIPT_CIDX, "Script"}
    };

    void AddComponent(int idx, entt::entity e)
    {
        m_Console.put(MessageType::INFO, std::format("Added Component '{}' to '{}'", COMPONENT_MAP.at(idx), m_Scene->m_Registry.get<Sauce::UUIDComponent>(e).Name));
        switch (idx)
        {
        case TRANSFORM_CIDX:
            m_Scene->m_Registry.emplace_or_replace<Sauce::TransformComponent>(e);
            break;
        case CAMERA_CIDX:
            m_Scene->m_Registry.emplace_or_replace<Sauce::CameraComponent>(e);
            break;
        case MESH_INSTANCE_CIDX:
            m_Scene->m_Registry.emplace_or_replace<Sauce::MeshInstance>(e);
            m_Scene->m_Registry.get<Sauce::MeshInstance>(e).PMesh = Sauce::CreateCubeMesh();
            m_Scene->m_Registry.get<Sauce::MeshInstance>(e).IsVisible = true;
            break;
        case SCRIPT_CIDX:
            m_Scene->m_Registry.emplace_or_replace<Sauce::Script>(e);
            break;
        default:
            break;
        }
    }

    bool HasCoomponent(int idx, entt::entity e)
    {
        switch (idx)
        {
        case TRANSFORM_CIDX:
            return m_Scene->m_Registry.try_get<Sauce::TransformComponent>(e);
        case CAMERA_CIDX:
            return m_Scene->m_Registry.try_get<Sauce::CameraComponent>(e);
        case MESH_INSTANCE_CIDX:
            return m_Scene->m_Registry.try_get<Sauce::MeshInstance>(e);
        case SCRIPT_CIDX:
            return m_Scene->m_Registry.try_get<Sauce::Script>(e);
        default:
            return false;
        }
    }

    void DrawComponent(int idx, entt::entity e)
    {
        switch (idx)
        {
        case TRANSFORM_CIDX:
        {
            auto &t = m_Scene->m_Registry.get<Sauce::TransformComponent>(e);
            ImGui::DragFloat3("Position", glm::value_ptr(t.Translation), 0.05f);
            ImGui::DragFloat3("Rotation", glm::value_ptr(t.Rotation), 0.5f, -360.0f, 360.0f);
            ImGui::DragFloat3("Scale", glm::value_ptr(t.Scale), 0.05f, 0.001f, 100.0f);
            break;
        }
        case CAMERA_CIDX:
        {
            auto &c = m_Scene->m_Registry.get<Sauce::CameraComponent>(e);
            ImGui::DragFloat("FOV", &c.FOV, 0.05f);
            ImGui::DragInt("Width", (int*)&c.Width, 1);
            ImGui::DragInt("Height", (int*)&c.Height, 1);
            ImGui::Checkbox("Main", &c.IsMain);
            ImGui::Checkbox("Adjustable", &c.IsAdjustable);
            break;
        }
        case MESH_INSTANCE_CIDX:
        {
            ImGui::Text("Mesh:");
            break;
        }
        case SCRIPT_CIDX:
        {
            ImGui::Text("Path:");
            break;
        }
        default:
            break;
        }
    }

    void DrawInspector()
    {
        if(!m_ViewStates.inspector)
            return;
        ImGui::Begin("Inspector", &m_ViewStates.inspector);
            if(m_Scene->m_Registry.try_get<Sauce::UUIDComponent>(m_SelectedEntity))
            {
                auto uuid = m_Scene->m_Registry.get<Sauce::UUIDComponent>(m_SelectedEntity);
                ImGui::Text("Name: %s", uuid.Name.c_str());
                ImGui::Text("UUID: %s", std::to_string((uint64_t)uuid.Id).c_str());
                ImGui::Separator();
                ImGui::Text("Components:");

                for(int i = 0; i < COMPONENT_MAP.size(); i++)
                if(HasCoomponent(i, m_SelectedEntity))
                {
                    if(ImGui::CollapsingHeader(COMPONENT_MAP.at(i).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
                    {
                        DrawComponent(i, m_SelectedEntity);
                    }
                }

                ImGui::Separator();
                if(ImGui::Button("Add Component"))
                {
                    ImGui::OpenPopup("Add Component");
                }

                if(ImGui::BeginPopup("Add Component"))
                {
                    for(int i = 0; i < COMPONENT_MAP.size(); i++)
                    {
                        if(ImGui::Button(COMPONENT_MAP.at(i).c_str()))
                        {
                            AddComponent(i, m_SelectedEntity);
                            ImGui::CloseCurrentPopup();
                        }
                    }
                    ImGui::EndPopup();
                }
            }
        ImGui::End();
    }

    void DrawItems(uint32_t width, uint32_t height)
    {
        glm::mat4 view = {1.0f};
        glm::mat4 proj = {1.0f};

        view = glm::lookAt(m_EditorTransform.Translation, m_EditorTransform.Translation + m_EditorTransform.Rotation, m_EditorCam.Up);
        proj = glm::perspective(glm::radians(m_EditorCam.FOV), (float)width / (float)height, F_NEAR, F_FAR);

        auto meshView = m_Scene->m_Registry.view<Sauce::MeshInstance>();
        for(auto entity : meshView)
        {
            if(!m_Scene->m_Registry.try_get<Sauce::TransformComponent>(entity) || m_Scene->m_Registry.get<Sauce::MeshInstance>(entity).PMesh == nullptr || !m_Scene->m_Registry.get<Sauce::MeshInstance>(entity).IsVisible) continue;
            m_Renderer->Submit(m_Scene->m_Registry.get<Sauce::TransformComponent>(entity), view, proj, meshView->get(entity).PMesh);
        }
    }

    void UpdateInput()
    {
        float x = 0, z = 0;
        if(inputSystem.GetKeyDown(GLFW_KEY_S))
        {
            z += 2 * m_DT;
        }
        if(inputSystem.GetKeyDown(GLFW_KEY_W))
        {
            z -= 2 * m_DT;
        }
        if(inputSystem.GetKeyDown(GLFW_KEY_D))
        {
            x += 2 * m_DT;
        }
        if(inputSystem.GetKeyDown(GLFW_KEY_A))
        {
            x -= 2 * m_DT;
        }
        m_EditorTransform.Translate({x, 0.0f, z});
    }

    void DrawViewport()
    {
        if (!m_ViewStates.viewport) return;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("Viewport", &m_ViewStates.viewport, ImGuiWindowFlags_NoScrollbar);
            ImGui::PopStyleVar();

            ImVec2 regionSize = ImGui::GetContentRegionAvail();
            ImVec2 originScreen = ImGui::GetCursorScreenPos();

            if(m_FrameBuffer->GetWidth() != (int)regionSize.x || m_FrameBuffer->GetHeight() != (int)regionSize.y)
            {
                m_FrameBuffer->Resize((int)regionSize.x, (int)regionSize.y);
            }

            m_FrameBuffer->Bind();
                glViewport(0, 0, (int)regionSize.x, regionSize.y);
                m_Renderer->Clear({0.5f, 0.2f, 0.6f, 1.0f});
                if(ImGui::IsWindowFocused()) UpdateInput();
                DrawItems((int)regionSize.x, (int)regionSize.y);
            m_FrameBuffer->Unbind();

            glViewport(0, 0, m_Renderer->GetWinWidth(), m_Renderer->GetWinHeight());
            ImTextureID texId = (ImTextureID)(intptr_t)m_FrameBuffer->GetColorAttachment();
            ImGui::Image(texId, regionSize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();
    }

};

void Edit(const std::string &path)
{
    Editor e = {path};
    e.Run();
}

class ProjectSelector
{
public:
    ProjectSelector() {}
    ~ProjectSelector() {}

    std::string Run()
    {
        m_Renderer.CreateWindow("Sauce Editor", 800, 800, on_close,key_callback, mouse_button_callback, scroll_callback, char_callback, enter_callback, cursor_pos_callback, window_focus_callback);
        m_Layer.Init(m_Renderer.GetWindow());

        bool shouldClose = false;
        bool open = false, create = false;
        char textBuffer[256];
        char nameBuffer[256];
        std::string errorText = "";
        nfdchar_t *nfdPath;
        bool success = false;

        while(!shouldClose)
        {
            shouldClose = glfwWindowShouldClose(m_Renderer.GetWindow());
            m_Renderer.Clear();
            glfwPollEvents();

            if (glfwGetWindowAttrib(m_Renderer.GetWindow(), GLFW_ICONIFIED) != 0)
            {
                ImGui_ImplGlfw_Sleep(10);
                continue;
            }
#pragma region Draw
            m_Layer.BeginFrame();
                ImGui::Begin("Choose Project", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
                    if(ImGui::Button("Open"))
                        ImGui::OpenPopup("Open");
                    ImGui::SameLine();
                    if(ImGui::Button("Create"))
                        ImGui::OpenPopup("Create");
                    ImGui::SameLine();
                    if(ImGui::Button("Quit"))
                        return "exit";

                    if(ImGui::BeginPopup("Open"))
                    {
                        ImGui::InputText("Path", textBuffer, sizeof textBuffer);
                        ImGui::SameLine();
                        if(ImGui::Button("Browse"))
                        {
                            nfdresult_t res = NFD_PickFolder("/home", &nfdPath);
                            if(res == NFD_OKAY)
                            {
                                snprintf(textBuffer, sizeof(textBuffer), "%s", nfdPath);
                                free(nfdPath);
                                nfdPath = nullptr;
                            }
                        }
                        ImGui::Separator();
                        if(ImGui::Button("Open"))
                        {
                            if(std::filesystem::exists(std::format("{}/.project_info.json", std::string(textBuffer))))
                            {
                                ImGui::CloseCurrentPopup();
                                m_Layer.Shutdown();
                                m_Renderer.KillWindow();
                                return {textBuffer};
                            }
                            errorText = std::format("invalid path '{}'", std::string(textBuffer));
                            ImGui::OpenPopup("Error");
                        }
                        ImGui::SameLine();
                        if(ImGui::Button("Cancel"))
                            ImGui::CloseCurrentPopup();
                        ImGui::EndPopup();
                    }

                    if(ImGui::BeginPopup("Create"))
                    {
                        ImGui::InputText("Path", textBuffer, sizeof textBuffer);
                        ImGui::SameLine();
                        if(ImGui::Button("Browse"))
                        {
                            nfdresult_t res = NFD_PickFolder("/home", &nfdPath);
                            if(res == NFD_OKAY)
                            {
                                snprintf(textBuffer, sizeof(textBuffer), "%s", nfdPath);
                                free(nfdPath);
                                nfdPath = nullptr;
                            }
                        }

                        ImGui::InputText("Name", nameBuffer, sizeof nameBuffer);

                        ImGui::Separator();
                        if(ImGui::Button("Create"))
                        {
                            bool error = false;
                            if(!std::filesystem::exists(std::string(textBuffer)))
                            {
                                errorText = std::format("path '{}' is no valid directorry", std::string(textBuffer));
                                error = true;
                                ImGui::OpenPopup("Error");
                            }
                            if(std::string(nameBuffer).empty())
                            {
                                error = true;
                                errorText = std::format("invalid name '{}'", std::string(nameBuffer));
                                ImGui::OpenPopup("Error");
                            }

                            if(!error)
                            {
                                createProject({nameBuffer}, {textBuffer});
                                ImGui::CloseCurrentPopup();
                                m_Layer.Shutdown();
                                m_Renderer.KillWindow();
                                return std::format("{}/{}", std::string(textBuffer), std::string(nameBuffer));
                            }
                        }

                        ImGui::EndPopup();
                    }

                    if(ImGui::BeginPopup("Error"))
                    {
                        if(errorText.empty())
                            ImGui::CloseCurrentPopup();

                        ImGui::TextColored({255, 0, 0, 255}, errorText.c_str());
                        if(ImGui::Button("OK"))
                        {
                            errorText = "";
                            ImGui::CloseCurrentPopup();
                        }
                        ImGui::EndPopup();
                    }
                ImGui::End();
            m_Layer.EndFrame();
#pragma endregion

        glfwSwapBuffers(m_Renderer.GetWindow());
        }
        m_Layer.Shutdown();
        m_Renderer.KillWindow();
        return "exit";
    }
private:
    Sauce::Renderer m_Renderer;
    ImGuiLayer m_Layer;
};

int main(int argc, char **argv)
{
    if(argc == 1)
    {
        ProjectSelector selector;
        std::string path = selector.Run();

        if(path == "exit")
            return 0;

        if(!std::filesystem::exists(std::format("{}/.project_info.json", path)))
        {
            std::runtime_error(std::format("path {} is no valid project path", path));
        }

        Edit(path);
    }
    else if(std::filesystem::exists(std::format("{}/.project_info.json", argv[1])))
    {
        Edit(argv[1]);
    }
}
