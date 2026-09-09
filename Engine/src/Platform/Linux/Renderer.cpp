#include <cassert>
#include <glad/glad.h>
#include <print>

#include <format>
#include <memory>

#include "Renderer.h"

#include "Engine/vendor/utils_hpp/escapes.hpp"

#include "GLFW/glfw3.h"

#include "Input/InputSystem.h"

namespace Sauce
{
	void LinuxRenderer::InitWindow(const std::string &title, uint32_t width, uint32_t height, std::shared_ptr<InputSystem> inputSystem)
	{
        m_Title = title;
        m_Width = width;
        m_Height = height;
        m_InputSystem = std::move(inputSystem);

        glfwSetErrorCallback([](int error, const char *desc) { std::println("{} {}: {}", Escapes::ColoredText("[GLFW ERROR]", Escapes::RED), error, desc); });
        assert(glfwInit() && std::format("{} Failed to initialize GLFW", Escapes::ColoredText("[GLFW ERROR]", Escapes::RED)).c_str());

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_DEPTH_BITS, 24);

        m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);

        if(!m_Window)
        {
        	glfwTerminate();
        	assert(false && std::format("{} Failed to create window", Escapes::ColoredText("[GLFW ERROR]", Escapes::RED)).c_str());
        }
        glfwMakeContextCurrent(m_Window);

#pragma region Callbacks

        glfwSetWindowUserPointer(m_Window, this);

        glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow *window, int width, int height) {
        		auto *self = static_cast<LinuxRenderer *>(glfwGetWindowUserPointer(window));
        		self->m_Width = width;
        		self->m_Height = height;
        		glViewport(0, 0, width, height);
        		});

        glfwSetWindowUserPointer(m_Window, m_InputSystem.get());

        glfwSetKeyCallback(m_Window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        		auto input = static_cast<InputSystem *>(glfwGetWindowUserPointer(window));
        		input->KeyCallback(window, key, scancode, action, mods);
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow *window, int button, int action, int mods) {
        		auto input = static_cast<InputSystem *>(glfwGetWindowUserPointer(window));
        		input->MouseButtonCallback(window, button, action, mods);
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow *window, double xoffset, double yoffset){
        		auto input = static_cast<InputSystem *>(glfwGetWindowUserPointer(window));
                input->ScrollCallback(window, xoffset, yoffset);
        });

        glfwSetCharCallback(m_Window, [](GLFWwindow *window, unsigned int codepoint){
        		auto input = static_cast<InputSystem *>(glfwGetWindowUserPointer(window));
                input->CharCallback(window, codepoint);
        });

        glfwSetCursorEnterCallback(m_Window, [](GLFWwindow *window, int entered){
        		auto input = static_cast<InputSystem *>(glfwGetWindowUserPointer(window));
                input->EnterCallback(window, entered);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow *window, double xpos, double ypos){
        		auto input = static_cast<InputSystem *>(glfwGetWindowUserPointer(window));
                input->CursorPosCallback(window, xpos, ypos);
        });

        glfwSetWindowFocusCallback(m_Window, [](GLFWwindow *window, int focused){
        		auto input = static_cast<InputSystem *>(glfwGetWindowUserPointer(window));
                input->WindowFocusCallback(window, focused);
        });
#pragma endregion
		if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			glfwTerminate();
			assert(false && std::format("{} faild to load GLAD", Escapes::ColoredText("[GLAD ERROR]", Escapes::RED)).c_str());
		}
		glfwSwapInterval(0);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glDisable(GL_CULL_FACE);
    }

	void LinuxRenderer::KillWindow()
	{
		glfwTerminate();
	}

    void LinuxRenderer::Clear(glm::vec4 col)
    {
        glClearColor(col.r, col.g, col.b, col.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

	void LinuxRenderer::Draw()
	{
        Clear({1.0f, 0.0f, 0.2f, 1.0f});

        glfwSwapBuffers(m_Window);
        glfwPollEvents();
        if (glfwWindowShouldClose(m_Window))
            m_OnShouldClose();
	}

	void LinuxRenderer::SetShader(const Shader &shader)
	{
		m_Shader = std::move(shader);
	}

	uint32_t LinuxRenderer::GetWinWidth()
	{
		return m_Width;
	}

	uint32_t LinuxRenderer::GetWinHeight()
	{
		return m_Height;
	}
}
