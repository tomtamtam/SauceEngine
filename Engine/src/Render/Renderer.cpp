#include "Renderer.h"
#include "Render/Shader.h"
#include "Render/Window.h"
#include <cstdint>
#include <iostream>
#include <memory>

namespace Sauce
{
    void Renderer::CreateWindow(const std::string &name, uint32_t width, uint32_t height)
    {
        m_Window = std::make_unique<Window>(name, width, height, m_OnShouldClose);;
        m_Window->Init();
    }

    void Renderer::KillWindow()
    {
        m_Window->Destroy();
    }
 
    void Renderer::BeginDraw()
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        m_CurrentShader->Bind();
    }
 
    void Renderer::Draw()
    {
        m_Window->Update();
    }

    Renderer::Renderer(std::function<void()> onShouldClose)
        : m_OnShouldClose(onShouldClose)
    {
        std::cout << "c0onstructor\n";
    }
 
    Renderer::~Renderer()
    {
    }

    template<>
    void Renderer::Set<std::shared_ptr<Shader>>(std::shared_ptr<Shader> shader)
    {
        m_CurrentShader = std::move(shader);
    }

    template<>
    void Renderer::Set<RenderType>(RenderType type)
    {
        m_CurrentType = type;
    }
}
