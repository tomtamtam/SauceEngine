#pragma once

#include "Editor/UI/Core/Context.h"

#include "Editor/UI/Panels/Panel.h"

class ToolBarPanel : public Panel
{
public:
	ToolBarPanel(EditorContext &ctx);
	void OnImGui() override;
	void OnUpdate() override;
};
