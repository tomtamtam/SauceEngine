#pragma once

#include <memory>

#include "Core/SceneSerializer.h"

#include "ECS/Scene.h"

#include "Editor/UI/ImGuiLayer.h"

#include "Editor/UI/Core/Actions.h"
#include "Editor/UI/Core/CommandStack.h"
#include "Editor/UI/Core/Console.h"
#include "Editor/UI/Core/EditorCam.h"
#include "Editor/UI/Core/Project.h"

#include "Platform/Linux/ImGui/Renderer.h"

#include "Render/Framebuffer.h"
#include "Render/Shader.h"

#include "entt/entity/fwd.hpp"

constexpr std::size_t MAX_UNDO_DEPTH { 265 };

class EditorContext
{
public:
	EditorContext()
		: renderer(std::make_shared<Sauce::Linux::ImRenderer>()),
		  inputSystem(std::make_shared<Sauce::InputSystem>()),
		  scene(std::make_shared<Sauce::Scene>(renderer, inputSystem)),
		  sceneSerializer(std::make_shared<Sauce::SceneSerializer>(scene)),
		  console(),
		  commandStack(MAX_UNDO_DEPTH)
	{}

	~EditorContext() = default;

	std::shared_ptr<Sauce::Scene> scene;
	std::shared_ptr<Sauce::Linux::ImRenderer> renderer;
	std::shared_ptr<Sauce::InputSystem> inputSystem;
	std::shared_ptr<Sauce::SceneSerializer> sceneSerializer;
	ImGuiLayer layer;
	std::unique_ptr<Sauce::Shader> currentShader;
	std::shared_ptr<Sauce::FrameBuffer> frameBuffer;
	entt::entity entity;

	Console console;
	Project project;
	CommandStack commandStack;
	Actions actions;

	EditorCam cam;

	float dt = 0;
};
