#include <memory>
#include <string>

#include "EditorApp.h"

#include "imgui.h"

#include "../Core/Console.h"
#include "../Core/Context.h"

#include "Editor/UI/Core/Actions.h"

#include "Editor/UI/Panels/AssetBrowserPanel.h"
#include "Editor/UI/Panels/ConsolePanel.h"
#include "Editor/UI/Panels/FPSPanel.h"
#include "Editor/UI/Panels/InspectorPanel.h"
#include "Editor/UI/Panels/MainWindow.h"
#include "Editor/UI/Panels/RunWindowPanel.h"
#include "Editor/UI/Panels/SceneHierarchyPanel.h"
#include "Editor/UI/Panels/ToolBarPanel.h"
#include "Editor/UI/Panels/ViewportPanel.h"

#include "Render/Framebuffer.h"

	constexpr size_t MAIN_WINDOW_IDX { 0 };
	constexpr size_t TOOL_BAR_PANEL_IDX { 1 };
	constexpr size_t FPSPANEL_PANEL_IDX { 2 };
	constexpr size_t RUN_WINDOW_PANEL_IDX { 3 };
	constexpr size_t SCENE_HIERARCHY_PANEL_IDX { 4 };
	constexpr size_t INSPECTOR_PANEL_IDX { 5 };
	constexpr size_t CONSOLE_PANEL_IDX { 6 };
	constexpr size_t ASSET_BROWSER_PANEL_IDX { 7 };
	constexpr size_t VIEWPORT_PANEL_IDX { 8 };

EditorApp::EditorApp(EditorContext &ctx)
	: m_Context(ctx)
{
	AddPanel<MainWindow>([this]{ m_Panels[TOOL_BAR_PANEL_IDX]->OnImGui(); });
	AddPanel<ToolBarPanel>();
	AddPanel<FPSPanel>(&m_Context.dt);
	AddPanel<RunWindowPanel>();
	AddPanel<SceneHierarchyPanel>();
	AddPanel<InspectorPanel>();
	AddPanel<ConsolePanel>();
	AddPanel<AssetBrowserPanel>();
	AddPanel<ViewportPanel>();
	
	AssignActions();
}

void EditorApp::Run(const std::string &path)
{
	m_Context.project.Validate(path);
	m_Context.console.Put(MessageType::INFO, "Editing " + path);

    m_Context.renderer->InitWindow(m_Context.project.name, 800, 800, m_Context.inputSystem);
    m_Context.scene->p_AssetSystem.emplace();
    m_Context.layer.Init(m_Context.renderer->GetWindow());

    m_Context.currentShader = std::make_unique<Sauce::Shader>(m_Context.project.path + '/' + m_Context.project.shaderPath);
    m_Context.renderer->SetShader(*m_Context.currentShader);
    m_Context.frameBuffer = std::make_shared<Sauce::FrameBuffer>(200, 200);
	m_Context.frameBuffer->Unbind();

    m_Context.cam.cam = {{0.0f, 1.0f, 0.0f}, 60.0f, 200, 200, true, false};
    m_Context.cam.transform.Translation = {0.0f, 0.0f, 3.0f};
    m_Context.cam.transform.Rotation = {0.0f, 0.0f, -1.0f};

    m_Context.sceneSerializer->Deserialze(m_Context.project.path + '/' + m_Context.project.currentScene);
    m_Context.console.Put(MessageType::INFO, std::format("Loaded Scene '{}'", m_Context.project.path));
	m_Context.scene->p_AssetSystem->LoadAssets();

	m_ShouldClose = false;

    m_BeginTime = glfwGetTime();
    m_EndTime = glfwGetTime();
    m_Context.dt = -1.0f;

    while(!m_ShouldClose)
    {
		Update();
    }

    m_Context.layer.Shutdown();
	m_Context.renderer->KillWindow();
}

void EditorApp::Update()
{
   if(m_Context.dt >= 0.0f)
   {
       m_ShouldClose = m_Context.renderer->WindowShouldClose();
       m_Context.renderer->Clear();

       glfwPollEvents();

       if (glfwGetWindowAttrib(m_Context.renderer->GetWindow(), GLFW_ICONIFIED) != 0)
       {
           ImGui_ImplGlfw_Sleep(10);
           return;
       }

       m_Context.layer.BeginFrame();
			m_Context.inputSystem->BeginFrame();
           	UpdatePanels();
           	DrawPanels();
       		m_Context.inputSystem->EndFrame();
       m_Context.layer.EndFrame();
       glfwSwapBuffers(m_Context.renderer->GetWindow());
   }
   m_EndTime = glfwGetTime();
   m_Context.dt = m_EndTime - m_BeginTime;
   m_BeginTime = m_EndTime;
}

void EditorApp::UpdatePanels()
{
	for(auto &p : m_Panels)
	{
		p->OnUpdate();
	}
}

void EditorApp::DrawPanels()
{
	for(auto &p : m_Panels)
	{
		if(p->open && p->Title() != "ToolBarPanel")
			p->OnImGui();
	}
}

void EditorApp::AssignActions()
{
	m_Context.actions.Register(ActionType::QUIT, {
			ActionGroup::FILE,
			"Quit SauceEditor",
			"Ctrl+Q",
			ImGuiMod_Ctrl | ImGuiKey_Q,
			[this]{ m_ShouldClose = true; },
			[this] { return true; }});

	m_Context.actions.Register(ActionType::SAVE, {
			ActionGroup::FILE,
			"Save active Scene",
			"Ctrl+S",
			ImGuiMod_Ctrl | ImGuiKey_S,
			[this]{ m_Context.sceneSerializer->Deserialze(m_Context.project.path + '/' + m_Context.project.currentScene);
					m_Context.console.Put(MessageType::INFO, "Saved Scene '" + m_Context.project.currentScene + "'"); },
			[this] { return m_Context.commandStack.IsDirty(); },
			});

	m_Context.actions.Register(ActionType::RUN, {
			ActionGroup::PROJECT,
			"Run default Scene",
			"F5",
			ImGuiKey_F5,
			[this]{ m_Context.console.Put(MessageType::INFO, "Running default Scene"); },
			[this] { return true; }});

	m_Context.actions.Register(ActionType::RUN_CURRENT, {
			ActionGroup::PROJECT,
			"Run current Scene",
			"F6",
			ImGuiKey_F6,
			[this]{ m_Context.console.Put(MessageType::INFO, "Running current Scene"); },
			[this] { return true; }});

	m_Context.actions.Register(ActionType::COMPILE, {
			ActionGroup::PROJECT,
			"Compile Project",
			"F4",
			ImGuiKey_F4,
			[this]{ m_Context.console.Put(MessageType::INFO, "Compiling Project"); },
			[this] { return true; }});


	m_Context.actions.Register(ActionType::SHOW_RUN_WINDOW, {
			ActionGroup::VIEW,
			"Run Window",
			"Ctrl+Alt+R",
			ImGuiKey_R | ImGuiMod_Ctrl | ImGuiMod_Alt,
			[this]{ m_Panels[RUN_WINDOW_PANEL_IDX]->open = !m_Panels[RUN_WINDOW_PANEL_IDX]->open; },
			[this] { return true; },
			&m_Panels[RUN_WINDOW_PANEL_IDX]->open
			});

	m_Context.actions.Register(ActionType::SHOW_VIEWPORT, {
			ActionGroup::VIEW,
			"Viewport",
			"Ctrl+Alt+V",
			ImGuiKey_V | ImGuiMod_Ctrl | ImGuiMod_Alt,
			[this]{ m_Panels[VIEWPORT_PANEL_IDX]->open = !m_Panels[VIEWPORT_PANEL_IDX]->open; },
			[this] { return true; },
			&m_Panels[VIEWPORT_PANEL_IDX]->open
			});

	m_Context.actions.Register(ActionType::SHOW_SCENE_HIERARCHY, {
			ActionGroup::VIEW,
			"Scene Hierarchy",
			"Ctrl+Alt+H",
			ImGuiKey_H | ImGuiMod_Ctrl | ImGuiMod_Alt,
			[this]{ m_Panels[SCENE_HIERARCHY_PANEL_IDX]->open = !m_Panels[SCENE_HIERARCHY_PANEL_IDX]->open; },
			[this] { return true; },
			&m_Panels[SCENE_HIERARCHY_PANEL_IDX]->open
			});

	m_Context.actions.Register(ActionType::SHOW_INSPECTOR, {
			ActionGroup::VIEW,
			"Inspector",
			"Ctrl+Alt+I",
			ImGuiKey_I | ImGuiMod_Ctrl | ImGuiMod_Alt,
			[this]{ m_Panels[INSPECTOR_PANEL_IDX]->open = !m_Panels[INSPECTOR_PANEL_IDX]->open; },
			[this] { return true; },
			&m_Panels[INSPECTOR_PANEL_IDX]->open
			});

	m_Context.actions.Register(ActionType::SHOW_CONSOLE, {
			ActionGroup::VIEW,
			"Console",
			"Ctrl+Alt+C",
			ImGuiKey_C | ImGuiMod_Ctrl | ImGuiMod_Alt,
			[this]{ m_Panels[CONSOLE_PANEL_IDX]->open = !m_Panels[CONSOLE_PANEL_IDX]->open; },
			[this] { return true; },
			&m_Panels[CONSOLE_PANEL_IDX]->open
			});

	m_Context.actions.Register(ActionType::SHOW_ASSET_BROWSER, {
			ActionGroup::VIEW,
			"Asset Browser",
			"Ctrl+Alt+B",
			ImGuiKey_B | ImGuiMod_Ctrl | ImGuiMod_Alt,
			[this]{ m_Panels[ASSET_BROWSER_PANEL_IDX]->open = !m_Panels[ASSET_BROWSER_PANEL_IDX]->open; },
			[this] { return true; },
			&m_Panels[ASSET_BROWSER_PANEL_IDX]->open
			});

	m_Context.actions.Register(ActionType::SHOW_FPS_VIEW, {
			ActionGroup::VIEW,
			"FPS",
			"Ctrl+Alt+F",
			ImGuiKey_F | ImGuiMod_Ctrl | ImGuiMod_Alt,
			[this]{ m_Panels[FPSPANEL_PANEL_IDX]->open = !m_Panels[FPSPANEL_PANEL_IDX]->open; },
			[this] { return true; },
			&m_Panels[FPSPANEL_PANEL_IDX]->open
			});

	m_Context.actions.CalculateGroups();
}
