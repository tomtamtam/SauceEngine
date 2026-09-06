#include <cassert>
#include <cstdint>
#include <dlfcn.h>

#include <fstream>
#include <memory>
#include <string>

#include "Application.h"

#include "Core/SceneSerializer.h"

#include "GLFW/glfw3.h"

#include "Render/Renderer.h"
#include "Render/Shader.h"

#include "Util/Json.h"

#include "nlohmann/json.hpp"

namespace Sauce
{
    Application::Application()
        : m_IsRunning(true), m_Renderer(std::make_unique<Renderer>()), m_AssetSystem(), m_CurrentScene(std::make_shared<Scene>(m_Renderer)), m_SceneSerializer(m_CurrentScene)
    {
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

        auto self = shared_from_this();

        auto onShouldClose = [&self](){self->Terminate();};
        auto onResizeCallback = [&self](uint32_t width, uint32_t height) {};
        auto onKeyCallback = [&self](GLFWwindow *window, int key, int scancode, int action, int mods) {self->m_CurrentScene->GetInputSystem()->KeyCallback(window, key, scancode, action, mods);};
        auto onMouseButtonCallback = [&self](GLFWwindow *window, int button, int action, int mods) {};
        auto onScrollCallback = [&self](GLFWwindow *window, double xoffset, double yoffset) {};
        auto onCharCallback = [&self](GLFWwindow *window, uint32_t codepoint) {};
        auto onEnterCallback = [&self](GLFWwindow *window, int entered) {};
        auto onCursorPosCallback = [&self](GLFWwindow* window, double xpos, double ypos){};
        auto onWindowFocusCallback = [&self](GLFWwindow* window, int focused){};

        m_Renderer->CreateWindow(m_Name, 800, 800, onShouldClose, onKeyCallback, onMouseButtonCallback, onScrollCallback, onCharCallback, onEnterCallback,onCursorPosCallback, onWindowFocusCallback);

        m_CurrentScene->p_AssetSystem.emplace();

        m_SceneSerializer.Deserialze("../" + m_DefaultScene);

        m_CurrentScene->Init();

        auto shader = std::make_shared<Shader>("../"+m_DefaultShader);
        m_Renderer->SetMainShader(shader);
        
        //m_AssetSystem = {};
    }

    void Application::Terminate()
    {
        m_IsRunning = false;
    }


    void Application::Run()
    {
        Setup();

        m_BeginTime = glfwGetTime();
        m_EndTime = glfwGetTime();
        m_DT = -1.0f;

        m_CurrentScene->Start();

        while (m_IsRunning)
        {
            if(m_DT >= 0.0f)
            {
                m_CurrentScene->GetInputSystem()->BeginFrame();
                m_CurrentScene->Update(m_DT);
                m_Renderer->Draw();
                m_CurrentScene->GetInputSystem()->Endframe();
            }

            m_EndTime = glfwGetTime();
            m_DT = m_EndTime - m_BeginTime;
            m_BeginTime = m_EndTime;
        }

        m_Renderer->KillWindow();
    }
}
