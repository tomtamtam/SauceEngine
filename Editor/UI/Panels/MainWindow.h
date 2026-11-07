#pragma once

#include <functional>

#include "imgui_internal.h"

#include "Editor/UI/Core/Context.h"

#include "Editor/UI/Panels/Panel.h"

class MainWindow : public Panel
{
public:
	MainWindow(EditorContext &ctx, std::function<void()> drawMenubar)
		: m_InitializedDock(false), Panel(ctx, "MainWindow"), m_DrawMenuBar(drawMenubar)
	{}
	void OnImGui() override
	{
        m_Viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(m_Viewport->WorkPos);
        ImGui::SetNextWindowSize(m_Viewport->WorkSize);
        ImGui::SetNextWindowViewport(m_Viewport->ID);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("EditorDockSpaceHost", nullptr, HOST_FLAGS);
            ImGui::PopStyleVar(3);

            ImGuiID dockspaceId = ImGui::GetID(m_Title.c_str());
            ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
            if(!m_InitializedDock)
            {
                m_InitializedDock = true;

                ImGui::DockBuilderRemoveNode(dockspaceId);
                ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);
                ImGui::DockBuilderSetNodeSize(dockspaceId, m_Viewport->Size);


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
        	m_DrawMenuBar();
        ImGui::End();
    }

	void OnUpdate() override
	{
	}

private:
	ImGuiViewport *m_Viewport;
	std::function<void()> m_DrawMenuBar;
	bool m_InitializedDock;
};
