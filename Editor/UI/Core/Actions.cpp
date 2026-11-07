#include "Actions.h"


void Actions::Invoke(ActionType type)
{
	if(m_Actions[(size_t)type].enabled)
		m_Actions[(size_t)type].run();
}

void Actions::Register(ActionType type, Action action)
{
	m_Actions.insert_or_assign((size_t)type, std::move(action));
}

void Actions::ProcessInput()
{
	if(ImGui::GetIO().WantTextInput) // dont fire while typing in a field
    	return;
    for(size_t i = 0; i < m_Actions.size(); ++i)
        if(m_Actions[i].chord && ImGui::IsKeyChordPressed(m_Actions[i].chord))
            Invoke((ActionType)i);
}

Action &Actions::GetAction(ActionType type)
{
	return m_Actions.at((size_t)type);
}

void Actions::CalculateGroups()
{
	groupMap.clear();
	for(auto &pair : m_Actions)
	{
		auto type = (ActionType)pair.first;
		auto &action = pair.second;
		if(!groupMap.contains(action.group))
			groupMap.insert_or_assign(action.group, std::vector<ActionType>());

		groupMap[action.group].push_back(type);
	}
}
