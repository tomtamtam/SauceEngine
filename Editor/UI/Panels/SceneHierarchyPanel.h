#pragma once

#include "Editor/UI/Core/Context.h"

#include "Editor/UI/Panels/Panel.h"

class SceneHierarchyPanel : public Panel
{
public:
	SceneHierarchyPanel(EditorContext &ctx);
	void OnImGui() override;
	void OnUpdate() override;

	template<typename T>
	bool THasComponent(entt::entity e)
	{
	    return m_Ctx.scene->m_Registry.try_get<T>(e);
	}
	
	template<typename T>
	void DuplicateComponent(entt::entity e1, entt::entity e2)
	{
	    if(m_Ctx.scene->m_Registry.try_get<T>(e1))
	    {
	        T dupe = m_Ctx.scene->m_Registry.get<T>(e1);
	        m_Ctx.scene->m_Registry.emplace_or_replace<T>(e2);
			m_Ctx.scene->m_Registry.get<T>(e2) = dupe;
	    }
	    else
	    {
	        m_Ctx.console.Put(MessageType::ERROR, "Component to dupe does not exist");
	    }
	}

private:
	bool m_RequestChild;
	char m_TextBuffer[100];
};
