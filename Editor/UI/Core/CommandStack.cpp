#include <cstddef>

#include "CommandStack.h"


CommandStack::CommandStack(std::size_t maxDepth)
	: m_MaxDepth(maxDepth), m_Dirty(false)
{}


void CommandStack::Undo()
{
	m_LastCommands[m_LastCommands.size()]->Undo();
	m_LastCommands.erase(m_LastCommands.end());
}

void CommandStack::Execute(std::unique_ptr<Command> cmnd)
{
	cmnd->Do();
	Push(std::move(cmnd));
	m_Dirty = true;
}

void CommandStack::Push(std::unique_ptr<Command> cmnd)
{
	m_LastCommands.push_back(std::move(cmnd));
	if(m_LastCommands.size() > m_MaxDepth)
	{
		m_LastCommands.erase(m_LastCommands.begin());
	}

	m_Dirty = true;
}

bool CommandStack::IsDirty() const
{
	return m_Dirty;
}
