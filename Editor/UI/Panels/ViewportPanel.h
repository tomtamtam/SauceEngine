#pragma once

#include "Editor/UI/Core/Context.h"

#include "Editor/UI/Panels/Panel.h"

#include "glm/ext/vector_float2.hpp"

class ViewportPanel : public Panel
{
public:
	ViewportPanel(EditorContext &ctx);
	void OnImGui() override;
	void OnUpdate() override;
	void DrawItems(uint32_t width, uint32_t height);
	void UpdateInput();
private:
	glm::vec2 m_MousePos;
	glm::vec2 m_PrevMousePos;
};
