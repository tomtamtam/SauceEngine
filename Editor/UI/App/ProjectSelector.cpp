#include <filesystem>
#include <string>

#include "ProjectSelector.h"

#include "nfd.h"

#include "Editor/Shared/Shared.h"

ProjectSelector::ProjectSelector()
  : m_Renderer(std::make_shared<Sauce::Linux::ImRenderer>()), m_InputSystem(std::make_shared<Sauce::InputSystem>())
{}

std::string ProjectSelector::Run()
{
    m_Renderer->InitWindow("Sauce Editor", 800, 800, m_InputSystem);
    m_Layer.Init(m_Renderer->GetWindow());

    bool shouldClose = false;
    bool open = false, create = false;
    char textBuffer[256];
    char nameBuffer[256];
    std::string errorText = "";
    nfdchar_t *nfdPath;
    bool success = false;

    while(!shouldClose)
    {
        shouldClose = glfwWindowShouldClose(m_Renderer->GetWindow());
        m_Renderer->Clear();
        glfwPollEvents();

        if (glfwGetWindowAttrib(m_Renderer->GetWindow(), GLFW_ICONIFIED) != 0)
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
                            m_Renderer->KillWindow();
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
                            m_Renderer->KillWindow();
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

    glfwSwapBuffers(m_Renderer->GetWindow());
    }
    m_Layer.Shutdown();
    m_Renderer->KillWindow();
    return "exit";
}
