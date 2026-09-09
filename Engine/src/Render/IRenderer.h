#pragma once

#include <cstdint>
#include <glad/glad.h>

#include <memory>
#include <string>

#include "Input/InputSystem.h"

#include "Render/Mesh.h"
#include "Render/Shader.h"

#include "glm/ext/vector_float4.hpp"

namespace Sauce
{
	class IRenderer
	{
	public:
		virtual void InitWindow(const std::string &title, uint32_t width, uint32_t height, std::shared_ptr<InputSystem> inputSystem) = 0;
		virtual void KillWindow() = 0;
        virtual void Clear(glm::vec4 col = {0.1f, 0.1f, 0.1f, 1.0f}) = 0;
		virtual void Draw() = 0;
		virtual void SetShader(const Shader &shader) = 0;
		virtual uint32_t GetWinWidth() const = 0;
		virtual uint32_t GetWinHeight() const = 0;
		virtual bool WindowShouldClose() const = 0;

        virtual void Submit(const glm::mat4 &transform, const glm::mat4 &view, const glm::mat4 &proj, const Mesh &mesh) = 0;
	};
}
