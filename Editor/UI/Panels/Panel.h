#pragma once

#include <string>

#include "Editor/UI/Core/Context.h"

constexpr ImGuiWindowFlags HOST_FLAGS =
     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
     ImGuiWindowFlags_NoResize   | ImGuiWindowFlags_NoMove |
     ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
     ImGuiWindowFlags_MenuBar;

constexpr ImGuiWindowFlags FLOATING_FLAGS =
     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
     ImGuiWindowFlags_NoResize;

class Panel
{
public:
	Panel(EditorContext &ctx, const std::string &title)
		: m_Ctx(ctx), m_Title(title)
	{}
	virtual ~Panel() = default;
	virtual void OnImGui() = 0;
	virtual void OnUpdate() = 0;
	std::string Title() const { return m_Title; }
	bool open = true;
protected:
	std::string m_Title;
	EditorContext &m_Ctx;
};
