#include "ToolBarPanel.h"

#include "imgui.h"

#include "Editor/UI/Core/Actions.h"
#include "Editor/UI/Core/Context.h"

#include "Editor/UI/Panels/Panel.h"

ToolBarPanel::ToolBarPanel(EditorContext &ctx)
	: Panel(ctx, "ToolBarPanel")
{}

void ToolBarPanel::OnImGui()
{
	if(ImGui::BeginMenuBar())
	{
		for(auto &pair : m_Ctx.actions.groupMap)
		{
			auto &group = pair.first;
			auto &list = pair.second;

	    	if(ImGui::BeginMenu(GROUP_NAME_MAP.at((size_t)group).c_str()))
	    	{
	    		if(group == ActionGroup::VIEW)
	    		{
					for(auto &actionType : list)
					{
						auto &action = m_Ctx.actions.GetAction(actionType);
						ImGui::MenuItem(action.label.c_str(), nullptr, action.buffer.value());
					}
					ImGui::EndMenu();
	    			continue;
	    		}
				for(auto &actionType : list)
				{
					auto &action = m_Ctx.actions.GetAction(actionType);
					if(ImGui::MenuItem(action.label.c_str(), action.shortcutText.c_str()))
					{
						action.run();
					}
				}
				ImGui::EndMenu();
	    	}
		}
	    ImGui::EndMenuBar();
	}
}

void ToolBarPanel::OnUpdate()
{}
