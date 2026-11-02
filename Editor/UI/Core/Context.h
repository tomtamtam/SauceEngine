#pragma once

#include <memory>

#include "Core/SceneSerializer.h"

#include "ECS/Scene.h"

#include "Editor/UI/Core/Console.h"

#include "Platform/Linux/ImGui/Renderer.h"

class EditorContext
{
public:
	EditorContext()
		: renderer(std::make_shared<Sauce::Linux::ImRenderer>()),
		  inputSystem(std::make_shared<Sauce::InputSystem>()),
		  scene(std::make_shared<Sauce::Scene>(renderer, inputSystem)),
		  sceneSerializer(std::make_shared<Sauce::SceneSerializer>(scene)),
		  console()
	{}

	~EditorContext() = default;

	std::shared_ptr<Sauce::Scene> scene;
	std::shared_ptr<Sauce::Linux::ImRenderer> renderer;
	std::shared_ptr<Sauce::InputSystem> inputSystem;
	std::shared_ptr<Sauce::SceneSerializer> sceneSerializer;

	Console console;

	float dt = 0;
};
