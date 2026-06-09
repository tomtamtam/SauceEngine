#pragma once

#include "Render/Shader.h"
#include "Window.h"
#include <cstdint>
#include <functional>
#include <memory>

namespace Sauce
{
    class Renderer
    {
    public:
        Renderer(std::function<void()> onShouldClose);
        ~Renderer();

        void CreateWindow(const std::string &name, uint32_t width, uint32_t height);
        void KillWindow();

        void BeginDraw();
        void Draw();

        void SetShader(std::shared_ptr<Shader> shader);
    private:
        std::function<void()> m_OnShouldClose;
        std::unique_ptr<Window> m_Window;
        std::shared_ptr<Shader> m_CurrentShader;
    };
}
