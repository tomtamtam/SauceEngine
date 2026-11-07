#pragma once

#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

#include "ECS/Entity.h"

struct Command
{
virtual void Do() = 0;
virtual void Undo() = 0;
virtual ~Command() = default;
};

template<typename T>
struct AddComponentCommand : public Command
{
	AddComponentCommand(Sauce::Entity e)
		: entity(e)
	{
	}

	void Do() override
	{
		entity.AddComponent<T>();
	}

	void Undo() override
	{
		entity.RemoveComponent<T>();
	}

	Sauce::Entity entity;
	T component;
};

template<typename T>
struct RemoveComponentCommand : public Command
{
	RemoveComponentCommand(Sauce::Entity e)
		: entity(e)
	{
	}

	void Do() override
	{
		component = entity.GetComponent<T>();
		entity.RemoveComponent<T>();
	}

	void Undo() override
	{
		entity.AddComponent<T>();
		entity.GetComponent<T>() = component;
	}

	Sauce::Entity entity;
	T component;
};

class CommandStack
{
public:
	CommandStack(std::size_t maxDepth = 256);
	
	template<typename T, typename... Args>
	void Execute(Args &&...args)
	{
		Execute(std::make_unique<T>(std::forward<Args>(args)...));
	}

	void Undo();
	void MarkSaved();
	bool IsDirty() const;
private:
	void Execute(std::unique_ptr<Command> cmnd);
	void Push(std::unique_ptr<Command> cmnd);

	std::vector<std::unique_ptr<Command>> m_LastCommands;
	std::size_t m_MaxDepth;
	bool m_Dirty;
};
