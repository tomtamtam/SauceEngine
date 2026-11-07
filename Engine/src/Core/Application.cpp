#include <cassert>
#include <dlfcn.h>

#include <fstream>
#include <memory>
#include <string>

#include "Application.h"

#include "Core/Log.h"
#include "Core/SceneSerializer.h"

#include "GLFW/glfw3.h"

#include "Platform/Linux/OpenGL/Renderer.h"

#include "Render/Shader.h"

#include "Util/Json.h"

#include "nlohmann/json.hpp"

namespace Sauce
{
    Application::Application()
        : m_IsRunning(true), m_Renderer(std::make_shared<Linux::GLRenderer>()), m_InputSystem(std::make_shared<InputSystem>()), m_CurrentScene(std::make_shared<Scene>(m_Renderer, m_InputSystem)), m_SceneSerializer(m_CurrentScene)
    {
    	m_CurrentScene->SetInputSystem(m_InputSystem);
    }

    Application::~Application()
    {
    }


    void Application::Setup()
    {
        //Load projinfo
        std::ifstream info("../.project_info.json");
        json jInfo;
        info >> jInfo;

        m_Name = jInfo["name"];
        m_DefaultScene = jInfo["defaultScene"];
        m_DefaultShader = jInfo["defaultShader"];

        m_Renderer->InitWindow(m_Name, 800, 800, m_InputSystem);

        m_CurrentScene->p_AssetSystem.emplace();

        m_SceneSerializer.Deserialze("../" + m_DefaultScene);
        m_CurrentScene->p_AssetSystem->LoadAssets();

        m_CurrentScene->Init();

		m_Shader = std::make_unique<Shader>("../"+m_DefaultShader);
        m_Renderer->SetShader(*m_Shader);
        
    }

    void Application::Terminate()
    {
        m_IsRunning = false;
    }


    void Application::Run()
    {
		LOG_INFO("starting Application class");
        Setup();

        m_BeginTime = glfwGetTime();
        m_EndTime = glfwGetTime();
        m_DT = -1.0f;

        m_CurrentScene->Start();

		LOG_INFO("starting Application loop");
        while (m_IsRunning)
        {
            if(m_DT >= 0.0f)
            {
                glfwPollEvents();
                if(m_Renderer->WindowShouldClose())
                	Terminate();
                m_InputSystem->BeginFrame();
                m_Renderer->Clear();
                m_CurrentScene->Update(m_DT);
                m_Renderer->Draw();
                m_InputSystem->EndFrame();
            }

            m_EndTime = glfwGetTime();
            m_DT = m_EndTime - m_BeginTime;
            m_BeginTime = m_EndTime;
        }

        m_Renderer->KillWindow();
    }
}
