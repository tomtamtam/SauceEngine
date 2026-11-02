#include <string>

#include "EditorApp.h"

#include "Context.h"

#include "Editor/UI/Core/Console.h"

EditorApp::EditorApp(EditorContext &ctx)
	: m_Context(ctx)
{

}

void EditorApp::Run(const std::string &path)
{
	m_Context.console.Put(MessageType::INFO, "Editing " + path);
}
