#pragma once

#include <cstdint>
#include <glad/glad.h>

#include <functional>
#include <memory>
#include <optional>

#include "GLFW/glfw3.h"

#include "Input/InputSystem.h"

#include "Render/IRenderer.h"

namespace Sauce::Linux
{
	class GLRenderer : public IRenderer
	{
	public:
		GLRenderer();
		~GLRenderer();

		void InitWindow(const std::string &title, uint32_t width, uint32_t height, std::shared_ptr<InputSystem> inputSystem);
		void KillWindow();
        void Clear(glm::vec4 col = {0.1f, 0.1f, 0.1f, 1.0f});
		void Draw();
		void SetShader(const Shader &shader);
		uint32_t GetWinWidth() const;
		uint32_t GetWinHeight() const;
        void Submit(const glm::mat4 &transform, const glm::mat4 &view, const glm::mat4 &proj, const Mesh &mesh);
        inline GLFWwindow *GetWindow() const { return m_Window; }
        bool WindowShouldClose() const;
	private:
		uint32_t m_Width {}, m_Height {};
		GLFWwindow *m_Window;
		std::string m_Title {};
		std::shared_ptr<InputSystem> m_InputSystem;
		std::optional<Shader> m_Shader;
		std::function<void()> m_OnClose();
	};
}
