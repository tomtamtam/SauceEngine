#pragma once

#include <functional>
#include <optional>
#include <string>
#include <unordered_map>

#include "imgui.h"

enum class ActionType
{
	SAVE,
	QUIT,
	COMPILE,
	RUN,
	RUN_CURRENT,

	SHOW_RUN_WINDOW,
	SHOW_VIEWPORT,
	SHOW_SCENE_HIERARCHY,
	SHOW_INSPECTOR,
	SHOW_CONSOLE,
	SHOW_ASSET_BROWSER,
	SHOW_FPS_VIEW
};

enum class ActionGroup
{
	FILE,
	PROJECT,
	VIEW
};

const std::unordered_map<size_t, std::string> GROUP_NAME_MAP = {
	{(size_t)ActionGroup::PROJECT, "Project"},
	{(size_t)ActionGroup::FILE, "File"},
	{(size_t)ActionGroup::VIEW, "View"}
};

struct Action
{
	ActionGroup group;
	std::string label;
	std::string shortcutText;
	ImGuiKeyChord chord = 0;
	std::function<void()> run;
	std::function<bool()> enabled;
	std::optional<bool *> buffer;
};

class Actions
{
public:
	void Register(ActionType type, Action action);
	void Invoke(ActionType type);
	void ProcessInput();
	void CalculateGroups();
	Action &GetAction(ActionType type);

	std::unordered_map<ActionGroup, std::vector<ActionType>> groupMap;
private:
	std::unordered_map<size_t, Action> m_Actions;
};
