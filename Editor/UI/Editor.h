#pragma once

#include <string>

struct EditorState
{
    std::string projectPath;
    bool first;

    EditorState()
        : first(true)
    {}
    ~EditorState() {}
};

void Editor(EditorState *state);
