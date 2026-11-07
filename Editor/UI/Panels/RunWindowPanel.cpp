#include "RunWindowPanel.h"

#include "Editor/UI/Core/Context.h"

#include "Editor/UI/Panels/Panel.h"

RunWindowPanel::RunWindowPanel(EditorContext &ctx)
	: Panel(ctx, "AssetBrowserPanel")
{}

void RunWindowPanel::OnImGui()
{
   ImGui::SetNextWindowSize(ImVec2(140, 35), ImGuiCond_Always);
   ImGui::Begin("RunWindow", nullptr, FLOATING_FLAGS);
       if(ImGui::Button("Run"))
       {
			m_Ctx.actions.Invoke(ActionType::RUN);
       }
       ImGui::SameLine();
       if(ImGui::Button("Run current Scene"))
       {
			m_Ctx.actions.Invoke(ActionType::RUN_CURRENT);
       }
       ImGui::SameLine();
       if(ImGui::Button("Compile"))
       {
			m_Ctx.actions.Invoke(ActionType::COMPILE);
       }
   ImGui::End();
}

void RunWindowPanel::OnUpdate()
{
}
