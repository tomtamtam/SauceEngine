#include "AssetBrowserPanel.h"
#include "Editor/UI/Core/Context.h"
#include "Editor/UI/Panels/AssetBrowserPanel.h"
#include "Editor/UI/Panels/Panel.h"

AssetBrowserPanel::AssetBrowserPanel(EditorContext &ctx)
	: Panel(ctx, "AssetBrowserPanel")
{}

void AssetBrowserPanel::OnImGui()
{
    ImGui::Begin("Asset-Browser", &open);
    ImGui::End();
}

void AssetBrowserPanel::OnUpdate()
{
}
