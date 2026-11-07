//noincludeformat
#pragma once

#include <memory>

#include "Platform/Linux/ImGui/Renderer.h"

#include "../ImGuiLayer.h"



class ProjectSelector
{
public:
    ProjectSelector();
    ~ProjectSelector() = default;

    std::string Run();
private:
	std::shared_ptr<Sauce::Linux::ImRenderer> m_Renderer;
	std::shared_ptr<Sauce::InputSystem> m_InputSystem;
    ImGuiLayer m_Layer;
};
