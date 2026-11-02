#pragma once


#include "Editor/UI/Core/Context.h"

class EditorApp
{
public:
	EditorApp(EditorContext &ctx);
	~EditorApp() = default;

	void Run(const std::string &path);
private:
	EditorContext &m_Context;
};
