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
        Renderer();
        ~Renderer();

        void CreateWindow(const std::string &name, uint32_t width, uint32_t height, std::function<void()> onShouldClose, std::function<void(GLFWwindow *window, int key, int scancode, int action, int mod)> onKeyCallback);
        void KillWindow();

        void Clear();
        void Draw();

        void SetMainShader(std::shared_ptr<Shader>);

        void Submit(const glm::mat4 &transform, const glm::mat4 &view, const glm::mat4 &proj, Mesh *mesh);

        uint32_t GetWinWidth() const;
        uint32_t GetWinHeight() const;
    private:
        std::unique_ptr<Window> m_Window;
        std::shared_ptr<Shader> m_MainShader;
    };
}
