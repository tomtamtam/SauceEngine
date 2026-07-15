#pragma once

#include "Core/SceneSerializer.h"
#include "Render/Renderer.h"
#include "Render/Window.h"
#include <ECS/Scene.h>
#include <memory>
#include <string>

struct ProjectInfo
{
    std::string defaultScene;
    std::string defaultSader;
    std::string projectPath;
};

struct EditorState
{
    bool first;
    ProjectInfo info;

    std::shared_ptr<Sauce::Scene> scene;
    std::shared_ptr<Sauce::Renderer> renderer;

    Sauce::SceneSerializer sceneSerializer;

    EditorState()
        : first(true), renderer(std::make_shared<Sauce::Renderer>()), scene(std::make_shared<Sauce::Scene>(renderer)), sceneSerializer(scene)
    {
    }
    ~EditorState() {}
};

void Editor(EditorState *state);
