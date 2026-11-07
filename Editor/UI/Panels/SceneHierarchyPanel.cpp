#include "SceneHierarchyPanel.h"

#include "Editor/UI/Core/Context.h"

#include "Editor/UI/Panels/Panel.h"

SceneHierarchyPanel::SceneHierarchyPanel(EditorContext &ctx)
	: Panel(ctx, "AssetBrowserPanel"), m_RequestChild(false)
{}

std::string dupeVersionName(const std::string &name)
{
    std::string temp = name;
    char version = temp.at(temp.size() - 2);
    if (version == '9')
    {
        temp.at(temp.size() - 2) = '0';
    }
    else
    {
        temp.at(temp.size() - 2) = version + 1;
    }
    return temp;
}


void SceneHierarchyPanel::OnImGui()
{
    ImGui::Begin("Scene Hierarchy", &open);
        if(ImGui::Button("Add"))
        {
            ImGui::OpenPopup("Add Entity");
        }
        ImGui::Separator();
        ImGui::BeginChild("ScrollRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

        auto view = m_Ctx.scene->m_Registry.view<Sauce::UUIDComponent>();
        for(auto e : view)
        {
            ImGui::PushID((int)e);
            //ImGui::TextUnformatted(state.scene->m_Registry.get<Sauce::UUIDComponent>(e).Name.c_str());
            auto uuid = m_Ctx.scene->m_Registry.get<Sauce::UUIDComponent>(e);

            bool isSelected = (m_Ctx.entity == e);
            if(ImGui::Selectable(uuid.Name.c_str(), isSelected))
            {
                m_Ctx.entity = e;
            }
            if (ImGui::BeginPopupContextItem())
            {
                m_Ctx.entity = e;
                if (ImGui::MenuItem("Rename"))
                {
                    ImGui::OpenPopup("Rename Object");
                }
                if (ImGui::MenuItem("Duplicate"))
                {
                    std::string newName = m_Ctx.scene->m_Registry.get<Sauce::UUIDComponent>(m_Ctx.entity).Name;
                    if(newName.ends_with(')'))
                    {
                        newName = dupeVersionName(newName);
                    }
                    else
                    {
                        newName.push_back('(');
                        newName.push_back('1');
                        newName.push_back(')');
                    }
                    auto e = m_Ctx.scene->m_Registry.create();
                    m_Ctx.scene->m_Registry.emplace_or_replace<Sauce::UUIDComponent>(e, newName);

                    if(THasComponent<Sauce::TransformComponent>(m_Ctx.entity))
                    {
                        DuplicateComponent<Sauce::TransformComponent>(m_Ctx.entity, e);
                    }

                    if(THasComponent<Sauce::CameraComponent>(m_Ctx.entity))
                    {
                        DuplicateComponent<Sauce::CameraComponent>(m_Ctx.entity, e);
                    }

                    if(THasComponent<Sauce::Script>(m_Ctx.entity))
                    {
                        DuplicateComponent<Sauce::Script>(m_Ctx.entity, e);
                    }

                    if(THasComponent<Sauce::MeshInstance>(m_Ctx.entity))
                    {
                        DuplicateComponent<Sauce::MeshInstance>(m_Ctx.entity, e);
                    }
                }
                if (ImGui::MenuItem("Delete"))
                {
                    m_Ctx.scene->m_Registry.destroy(m_Ctx.entity);
                }
				if(ImGui::MenuItem("Add Child"))
				{
					m_RequestChild = true;
				}
                ImGui::EndPopup();
            }
			if(m_RequestChild)
			{
					ImGui::OpenPopup("Create Child");
					m_RequestChild = false;
			}
            if(ImGui::BeginPopup("Rename Object"))
            {
                char nameBuffer[128];
                ImGui::InputText("Name", nameBuffer, 128);
                if(ImGui::Button("Ok"))
                {
                    std::snprintf(nameBuffer, sizeof(nameBuffer), "%s", m_Ctx.scene->m_Registry.get<Sauce::UUIDComponent>(m_Ctx.entity).Name.c_str());
                }
                if(ImGui::Button("Cancel"))
                {
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        	if(ImGui::BeginPopup("Create Child"))
        	{
        	    ImGui::InputText("Name", m_TextBuffer, 100);
        	    if(ImGui::Button("Add"))
        	    {
        	        std::string name = { m_TextBuffer };
        	        std::shared_ptr<Sauce::Entity> e = m_Ctx.scene->CreateEntity(name);
					e->GetComponent<Sauce::UUIDComponent>().parent = m_Ctx.scene->m_Registry.get<Sauce::UUIDComponent>(m_Ctx.entity).Id;
					m_Ctx.scene->m_Registry.get<Sauce::UUIDComponent>(m_Ctx.entity).childeren.push_back(e->GetComponent<Sauce::UUIDComponent>().Id);
        	        m_Ctx.console.Put(MessageType::INFO, std::format("Added Child Object '{}' to '{}'", name, m_Ctx.scene->m_Registry.get<Sauce::UUIDComponent>(m_Ctx.entity).Name));
        	        ImGui::CloseCurrentPopup();
        	    }
        	    ImGui::SameLine();
        	    if(ImGui::Button("Cancel"))
        	    {
        	        ImGui::CloseCurrentPopup();
        	    }
        	    ImGui::EndPopup();
        	}
            ImGui::PopID();
        }
        ImGui::PopStyleVar();
        ImGui::EndChild();


        if(ImGui::BeginPopup("Add Entity"))
        {
            ImGui::InputText("Name", m_TextBuffer, 100);
            if(ImGui::Button("Add"))
            {
                std::string name = { m_TextBuffer };
                auto e = m_Ctx.scene->CreateEntity(name);
                m_Ctx.console.Put(MessageType::INFO, std::format("Added Object '{}'", name));
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if(ImGui::Button("Cancel"))
            {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    ImGui::End();
}

void SceneHierarchyPanel::OnUpdate()
{
}
