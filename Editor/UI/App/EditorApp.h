#pragma once

#include <utility>
#include <vector>

#include "../Core/Context.h"

#include "Editor/UI/Panels/Panel.h"

class EditorApp
{
public:
	EditorApp(EditorContext &ctx);
	~EditorApp() = default;

	void Run(const std::string &path);
private:
	void Update();
	void UpdatePanels();
	void DrawPanels();
	void AssignActions();

	template<typename T, typename... Args>
    T& AddPanel(Args&&... args)
    {
        auto panel = std::make_unique<T>(m_Context, std::forward<Args>(args)...);
        T& ref = *panel;
        m_Panels.push_back(std::move(panel));
        return ref;
    }

	EditorContext &m_Context;
	float m_BeginTime;
	float m_EndTime;
	bool m_ShouldClose;
    std::vector<std::unique_ptr<Panel>> m_Panels;
};
