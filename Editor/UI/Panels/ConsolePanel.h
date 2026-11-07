#pragma once

#include "Editor/UI/Core/Context.h"

#include "Editor/UI/Panels/Panel.h"

class ConsolePanel : public Panel
{
public:
	ConsolePanel(EditorContext &ctx);
	void OnImGui() override;
	void OnUpdate() override;
};
