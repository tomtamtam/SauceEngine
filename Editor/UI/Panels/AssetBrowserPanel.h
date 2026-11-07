#pragma once

#include "Editor/UI/Core/Context.h"

#include "Editor/UI/Panels/Panel.h"

class AssetBrowserPanel : public Panel
{
public:
	AssetBrowserPanel(EditorContext &ctx);
	void OnImGui() override;
	void OnUpdate() override;
};
