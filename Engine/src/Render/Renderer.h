#pragma once

#include "Render/Shader.h"
#include "Window.h"
#include <cstdint>
#include <functional>
#include <iostream>
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

        void BeginDraw();
        void Draw();


        template<typename T>
        void Set(T t)
        {
            std::cerr << "Unsupported Renderer::Set() Type\n";
        }
    private:
        std::function<void()> m_OnShouldClose;
        std::unique_ptr<Window> m_Window;
        std::shared_ptr<Shader> m_CurrentShader;
        RenderType m_CurrentType;
    };
}
