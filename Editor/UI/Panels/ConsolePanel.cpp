#include "ConsolePanel.h"

#include "Editor/UI/Core/Context.h"

ConsolePanel::ConsolePanel(EditorContext &ctx)
	: Panel(ctx, "Console")
{}

void ConsolePanel::OnImGui()
{
    ImGui::Begin(m_Title.c_str(), &open);
        if(ImGui::Button("Clear")) m_Ctx.console.clear();
        ImGui::SameLine();
        if(ImGui::Button("Copy All")) m_Ctx.console.copyAll();
        ImGui::SameLine();
        ImGui::Checkbox("Auto Scroll", &m_Ctx.console.autoScroll);
        ImGui::SameLine();
        ImGui::Checkbox("Show Warnings", &m_Ctx.console.showWarnings);

        ImGui::Separator();
        ImGui::BeginChild("ScrollRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

        for(auto msg : m_Ctx.console.messages)
        {
            if(msg.type == MessageType::WARNING && !m_Ctx.console.showWarnings) continue;

            ImVec4 color;
            std::string prefix;
            switch(msg.type)
            {
                case MessageType::INFO: color = ImVec4(0.0f, 0.6f, 0.6f, 1.0f); prefix = "[Info]"; break;
                case MessageType::WARNING: color = ImVec4(1.0f, 0.8f, 0.2f, 1.0f); prefix = "[Warning]"; break;
                case MessageType::ERROR: color = ImVec4(1.0f, 0.35f, 0.35f, 1.0f); prefix = "[ERROR]"; break;
                default: color = ImVec4(0.6f, 0.2, 0.6f, 1.0f); prefix = "[Debug]"; break;
            }

            ImGui::PushStyleColor(ImGuiCol_Text, color);
            ImGui::TextUnformatted(prefix.c_str());
            ImGui::PopStyleColor();
            ImGui::TextUnformatted(msg.text.c_str());
        }

        if(m_Ctx.console.autoScroll)
            ImGui::SetScrollHereY(1.0f);

        ImGui::PopStyleVar();
        ImGui::EndChild();
    ImGui::End();
}

void ConsolePanel::OnUpdate()
{
}
