//noincludeformat
#pragma once

#include <cstdint>

#include <functional>
#include <memory>

#include "Render/Mesh.h"
#include "Window.h"

#include "Render/Shader.h"

#include "glm/ext/matrix_float4x4.hpp"

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

        void CreateWindow(const std::string &name, uint32_t width, uint32_t height, std::function<void()> onShouldClose, SauceKeyCallback keyCallback, SauceMouseButtonCallback mouseButtonCallback, SauceScrollCallback scrollCallback, SauceCharCallback charCallback, SauceEnterCallback onEnterCallback, SauceCurtsorPositionCallback cursorPositionCallback, SauceWindowFocusCallback windowFocusCallback);
        void KillWindow();

        void Clear(glm::vec4 col = {0.1f, 0.1f, 0.1f, 1.0f});
        void Draw();

        void SetMainShader(std::shared_ptr<Shader>);

        void Submit(const glm::mat4 &transform, const glm::mat4 &view, const glm::mat4 &proj, const Mesh &mesh);

        uint32_t GetWinWidth() const;
        uint32_t GetWinHeight() const;
        GLFWwindow *GetWindow() const;
    private:
        std::unique_ptr<Window> m_Window;
        std::shared_ptr<Shader> m_MainShader;

        bool m_DrawExernal;
    };
}
