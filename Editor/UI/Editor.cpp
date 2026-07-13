#include "Editor.h"
#include "imgui.h"
#include <iostream>

void Editor(EditorState *state)
{
    if(state->first)
    {
        std:: cout << "\x1B[32mEditing\033[0m " << state->projectPath << '\n';
        state->first = false;
    }

    std::string name = "SauceEditor " + state->projectPath;

    auto *viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);

    ImGui::Begin(name.c_str());

    ImGui::End();
}
