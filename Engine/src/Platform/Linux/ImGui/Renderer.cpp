
#include <cassert>
#include <glad/glad.h>
#include <print>

#include <format>
#include <memory>

#include "Renderer.h"

#include "imgui_impl_glfw.h"

#include "Engine/src/Input/InputSystem.h"

#include "Engine/vendor/utils_hpp/escapes.hpp"

#include "GLFW/glfw3.h"

namespace Sauce::Linux
{
	void ImRenderer::InitWindow(const std::string &title, uint32_t width, uint32_t height, std::shared_ptr<InputSystem> inputSystem)
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
        		auto *self = static_cast<ImRenderer *>(glfwGetWindowUserPointer(window));
        		self->m_Width = width;
        		self->m_Height = height;
        		glViewport(0, 0, width, height);
        		});

        glfwSetKeyCallback(m_Window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        		auto self = static_cast<ImRenderer *>(glfwGetWindowUserPointer(window));

    			ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
        		self->m_InputSystem->KeyCallback(window, key, scancode, action, mods);
    			if (ImGui::GetIO().WantCaptureKeyboard)
    			    return;
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow *window, int button, int action, int mods) {
    			ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
        		auto self = static_cast<ImRenderer *>(glfwGetWindowUserPointer(window));
        		self->m_InputSystem->MouseButtonCallback(window, button, action, mods);
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow *window, double xoffset, double yoffset){
    			ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
        		auto self = static_cast<ImRenderer *>(glfwGetWindowUserPointer(window));
                self->m_InputSystem->ScrollCallback(window, xoffset, yoffset);
        });

        glfwSetCharCallback(m_Window, [](GLFWwindow *window, unsigned int codepoint){
    			ImGui_ImplGlfw_CharCallback(window, codepoint);
        		auto self = static_cast<ImRenderer *>(glfwGetWindowUserPointer(window));
                self->m_InputSystem->CharCallback(window, codepoint);
        });

        glfwSetCursorEnterCallback(m_Window, [](GLFWwindow *window, int entered){
    			ImGui_ImplGlfw_CursorEnterCallback(window, entered);
        		auto self = static_cast<ImRenderer *>(glfwGetWindowUserPointer(window));
                self->m_InputSystem->EnterCallback(window, entered);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow *window, double xpos, double ypos){
    			ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
        		auto self = static_cast<ImRenderer *>(glfwGetWindowUserPointer(window));
                self->m_InputSystem->CursorPosCallback(window, xpos, ypos);
        });

        glfwSetWindowFocusCallback(m_Window, [](GLFWwindow *window, int focused){
    			ImGui_ImplGlfw_WindowFocusCallback(window, focused);
        		auto self = static_cast<ImRenderer *>(glfwGetWindowUserPointer(window));
                self->m_InputSystem->WindowFocusCallback(window, focused);
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

	void ImRenderer::KillWindow()
	{
		glfwTerminate();
	}

    void ImRenderer::Clear(glm::vec4 col)
    {
        glClearColor(col.r, col.g, col.b, col.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

	void ImRenderer::Draw()
	{

	}

	void ImRenderer::SetShader(const Shader &shader)
	{
		m_Shader = std::move(shader);
	}

	uint32_t ImRenderer::GetWinWidth() const
	{
		return m_Width;
	}

	uint32_t ImRenderer::GetWinHeight() const
	{
		return m_Height;
	}

    bool ImRenderer::WindowShouldClose() const
    {
    	return glfwWindowShouldClose(m_Window);
    }

    void ImRenderer::Submit(const glm::mat4 &transform, const glm::mat4 &view, const glm::mat4 &proj, const Mesh &mesh)
    {
        mesh.Vao.Bind();
        mesh.Ibo.Bind();
        if(!m_Shader.has_value())
        {
			std::println("{} IRenderer::m_Shader has no value. Call IRenderer::SetShader", Escapes::ColoredText("[INTERNAL ERROR]", Escapes::RED));
        	return;
        }
        m_Shader->Bind();
        m_Shader->SetMatrix4("u_Transform", transform);
        m_Shader->SetMatrix4("u_View", view);
        m_Shader->SetMatrix4("u_Proj", proj);

        glDrawElements(GL_TRIANGLES, mesh.Ibo.getCount(), GL_UNSIGNED_INT, nullptr);
    }

	ImRenderer::ImRenderer()
	{}

	ImRenderer::~ImRenderer()
	{}
}
