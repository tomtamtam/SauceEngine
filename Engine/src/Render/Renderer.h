#pragma once

#include "Render/Mesh.h"
#include "Render/Shader.h"
#include "Window.h"
#include "glm/ext/matrix_float4x4.hpp"
#include <cstdint>
#include <functional>
#include <memory>

namespace Sauce
{
    enum RenderType
    {
        BATCH
    };

    #define ShaderType std::shared_ptr<Shader>

    class Renderer
    {
    public:
        Renderer(std::function<void()> onShouldClose);
        ~Renderer();

        void CreateWindow(const std::string &name, uint32_t width, uint32_t height);
        void KillWindow();

        void Clear();
        void Draw();

        void SetMainShader(std::shared_ptr<Shader>);

        void Submit(const glm::mat4 &transform, const glm::mat4 &cam, const glm::mat4 &camTransform, Mesh *mesh);
    private:
        std::function<void()> m_OnShouldClose;
        std::unique_ptr<Window> m_Window;
        std::shared_ptr<Shader> m_MainShader;
    };
}
