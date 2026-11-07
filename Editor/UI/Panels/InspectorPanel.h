#pragma once

#include "Editor/UI/Core/Context.h"

#include "Editor/UI/Panels/Panel.h"

class InspectorPanel : public Panel
{
public:
	InspectorPanel(EditorContext &ctx);
	void OnImGui() override;
	void OnUpdate() override;

	void DrawComponent(int idx, entt::entity e);
	bool HasCoomponent(int idx, entt::entity e);
	void AddComponent(int idx, entt::entity e);

	template<typename T>
	std::optional<T> GetComponentByUUID(Sauce::UUID uuid)
	{
		auto view = m_Ctx.scene->m_Registry.view<Sauce::UUIDComponent, T>();
		for(auto e : view)
		{
			if(m_Ctx.scene->m_Registry.get<Sauce::UUIDComponent>(e).Id == uuid)
			{
				return m_Ctx.scene->m_Registry.get<T>(e);
			}
		}
		return {};
	}
};
