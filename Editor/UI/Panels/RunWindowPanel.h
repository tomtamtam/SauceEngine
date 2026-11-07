#pragma once

#include "Editor/UI/Core/Context.h"

#include "Editor/UI/Panels/Panel.h"

class RunWindowPanel : public Panel
{
public:
	RunWindowPanel(EditorContext &ctx);
	void OnImGui() override;
	void OnUpdate() override;
};
