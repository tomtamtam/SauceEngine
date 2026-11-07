#pragma once

#include "Editor/UI/Core/Context.h"

#include "Editor/UI/Panels/Panel.h"

class FPSPanel : public Panel
{
public:
	FPSPanel(EditorContext &ctx, float *dt);
	void OnImGui() override;
	void OnUpdate() override;
private:
	float *m_Dt;
};
