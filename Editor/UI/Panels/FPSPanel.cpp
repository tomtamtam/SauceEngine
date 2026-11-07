#include "FPSPanel.h"

#include "Editor/UI/Core/Context.h"

#include "Editor/UI/Panels/Panel.h"

FPSPanel::FPSPanel(EditorContext &ctx, float *dt)
	: Panel(ctx, "AssetBrowserPanel"), m_Dt(dt)
{}

void FPSPanel::OnImGui()
{
	ImGui::Begin("FPS");
		ImGui::Text("Editor: %f FPS", 1 / *m_Dt);
	ImGui::End();
}

void FPSPanel::OnUpdate()
{
}
