#include "InspectorPanel.h"

#include "Editor/Shared/Shared.h"

#include "Editor/UI/Core/Context.h"

#include "Editor/UI/Panels/Panel.h"

#include "glm/gtc/type_ptr.hpp"

InspectorPanel::InspectorPanel(EditorContext &ctx)
	: Panel(ctx, "AssetBrowserPanel")
{}

const std::unordered_map<int, std::string> COMPONENT_MAP =
{
    {TRANSFORM_CIDX, "Transform"},
    {CAMERA_CIDX, "Camera"},
    {MESH_INSTANCE_CIDX, "Mesh Instance"},
    {SCRIPT_CIDX, "Script"}
};

void InspectorPanel::DrawComponent(int idx, entt::entity e)
{
    switch (idx)
    {
    case TRANSFORM_CIDX:
    {
        auto &t = m_Ctx.scene->m_Registry.get<Sauce::TransformComponent>(e);
        ImGui::DragFloat3("Position", glm::value_ptr(t.Translation), 0.05f);
        ImGui::DragFloat3("Rotation", glm::value_ptr(t.Rotation), 0.5f, -360.0f, 360.0f);
        ImGui::DragFloat3("Scale", glm::value_ptr(t.Scale), 0.05f, 0.001f, 100.0f);
        break;
    }
    case CAMERA_CIDX:
    {
        auto &c = m_Ctx.scene->m_Registry.get<Sauce::CameraComponent>(e);
        ImGui::DragFloat("FOV", &c.FOV, 0.05f);
        ImGui::DragInt("Width", (int*)&c.Width, 1);
        ImGui::DragInt("Height", (int*)&c.Height, 1);
        ImGui::Checkbox("Main", &c.IsMain);
        ImGui::Checkbox("Adjustable", &c.IsAdjustable);
        break;
    }
    case MESH_INSTANCE_CIDX:
    {
		if(!m_Ctx.scene->m_Registry.try_get<Sauce::MeshInstance>(e))
			break;

		std::vector<const char*> meshNames(m_Ctx.scene->p_AssetSystem->p_MeshCount);
		for (size_t i = 0; i < m_Ctx.scene->p_AssetSystem->p_MeshCount; i++)
		    meshNames[i] = m_Ctx.scene->p_AssetSystem->p_Meshes[i].name.c_str();

		auto& meshInstance {m_Ctx.scene->m_Registry.get<Sauce::MeshInstance>(e)};
		int meshIdx {static_cast<int>(meshInstance.meshId)};
		
		ImGui::Text("Mesh Instance");
		if (ImGui::Combo("Mesh", &meshIdx, meshNames.data(), static_cast<int>(meshNames.size()), 20))
		{
		    meshInstance.meshId = static_cast<uint32_t>(meshIdx);
		}
		ImGui::Checkbox("Visible", &meshInstance.IsVisible);
		break;
    }
    case SCRIPT_CIDX:
    {
		ImGui::Text("Name:");
		ImGui::Text("%s", m_Ctx.scene->m_Registry.get<Sauce::Script>(e).ScriptName.c_str());
        break;
    }
    default:
        break;
    }
}

bool InspectorPanel::HasCoomponent(int idx, entt::entity e)
{
    switch (idx)
    {
    case TRANSFORM_CIDX:
        return m_Ctx.scene->m_Registry.try_get<Sauce::TransformComponent>(e);
    case CAMERA_CIDX:
        return m_Ctx.scene->m_Registry.try_get<Sauce::CameraComponent>(e);
    case MESH_INSTANCE_CIDX:
        return m_Ctx.scene->m_Registry.try_get<Sauce::MeshInstance>(e);
    case SCRIPT_CIDX:
        return m_Ctx.scene->m_Registry.try_get<Sauce::Script>(e);
    default:
        return false;
    }
}

void InspectorPanel::AddComponent(int idx, entt::entity e)
{
    m_Ctx.console.Put(MessageType::INFO, std::format("Added Component '{}' to '{}'", COMPONENT_MAP.at(idx), m_Ctx.scene->m_Registry.get<Sauce::UUIDComponent>(e).Name));
    switch (idx)
    {
    case TRANSFORM_CIDX:
        m_Ctx.scene->m_Registry.emplace_or_replace<Sauce::TransformComponent>(e);
        break;
    case CAMERA_CIDX:
        m_Ctx.scene->m_Registry.emplace_or_replace<Sauce::CameraComponent>(e);
        break;
    case MESH_INSTANCE_CIDX:
        m_Ctx.scene->m_Registry.emplace_or_replace<Sauce::MeshInstance>(e);
        m_Ctx.scene->m_Registry.get<Sauce::MeshInstance>(e).meshId = Sauce::CUBE_UUID;
        m_Ctx.scene->m_Registry.get<Sauce::MeshInstance>(e).IsVisible = true;
        break;
    case SCRIPT_CIDX:
        m_Ctx.scene->m_Registry.emplace_or_replace<Sauce::Script>(e);
		//temp values
		m_Ctx.scene->m_Registry.get<Sauce::Script>(e).ScriptName = "Player";
		m_Ctx.scene->m_Registry.get<Sauce::Script>(e).ScriptPath = "./libGame.so";
        break;
    default:
        break;
    }
}

void InspectorPanel::OnImGui()
{
   ImGui::Begin("Inspector", &open);
       if(m_Ctx.scene->m_Registry.try_get<Sauce::UUIDComponent>(m_Ctx.entity))
       {
           auto uuid = m_Ctx.scene->m_Registry.get<Sauce::UUIDComponent>(m_Ctx.entity);
           ImGui::Text("Name: %s", uuid.Name.c_str());
           ImGui::Text("UUID: %s", std::to_string((uint64_t)uuid.Id).c_str());
   		if(uuid.parent)
   			ImGui::Text("Parent: [name] %s [uuid] %s", GetComponentByUUID<Sauce::UUIDComponent>(uuid.parent.value()).value_or(std::string({})).Name.c_str(), std::to_string((uint64_t)uuid.parent.value()).c_str());
           	ImGui::Separator();
           	ImGui::Text("Components:");

           	for(int i = 0; i < COMPONENT_MAP.size(); i++)
           	if(HasCoomponent(i, m_Ctx.entity))
           	{
            	if(ImGui::CollapsingHeader(COMPONENT_MAP.at(i).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
               	{
                   	DrawComponent(i, m_Ctx.entity);
               	}
           	}

           	ImGui::Separator();
           	if(ImGui::Button("Add Component"))
           	{
               	ImGui::OpenPopup("Add Component");
           	}

           	if(ImGui::BeginPopup("Add Component"))
           	{
           	    for(int i = 0; i < COMPONENT_MAP.size(); i++)
           	    {
           	        if(ImGui::Button(COMPONENT_MAP.at(i).c_str()))
           	        {
           	            AddComponent(i, m_Ctx.entity);
           	            ImGui::CloseCurrentPopup();
           	        }
           	    }
           	    ImGui::EndPopup();
           	}
       }
   ImGui::End();
}

void InspectorPanel::OnUpdate()
{
}
