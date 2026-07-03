#include "Renderer.h"
#include "Render/Buffers/IndexBuffer.h"
#include "Render/Buffers/VertexArray.h"
#include "Render/Buffers/VertexBuffer.h"
#include "Render/Shader.h"
#include "Render/Window.h"
#include "glm/ext/matrix_float4x4.hpp"
#include <cstdint>
#include <memory>

namespace Sauce
{
    void Renderer::CreateWindow(const std::string &name, uint32_t width, uint32_t height, std::function<void()> onShouldClose, std::function<void(GLFWwindow *window, int key, int scancode, int action, int mod)> onKeyCallback)
    {
        m_Window = std::make_unique<Window>(name, width, height, onShouldClose, onKeyCallback);
        m_Window->Init();
    }

    void Renderer::KillWindow()
    {
        m_Window->Destroy();
    }
 
    void Renderer::Clear()
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
 
    void Renderer::Draw()
    {
        m_Window->Update();
    }

    Renderer::Renderer()
    {
    }

    void Renderer::SetMainShader(std::shared_ptr<Shader> shader)
    {
        m_MainShader = shader;
    }
 
    Renderer::~Renderer()
    {
    }

    void Renderer::Submit(const glm::mat4 &transform, const glm::mat4 &view, const glm::mat4 &proj, Mesh *mesh)
    {
        mesh->Vao.Bind();
        mesh->Ibo.Bind();
        m_MainShader->Bind();
        m_MainShader->SetMatrix4("u_Transform", transform);
        m_MainShader->SetMatrix4("u_View", view);
        m_MainShader->SetMatrix4("u_Proj", proj);

        glDrawElements(GL_TRIANGLES, mesh->Ibo.getCount(), GL_UNSIGNED_INT, nullptr);
    }

    uint32_t Renderer::GetWinWidth() const
    {
        return m_Window->GetWidth();
    }
    uint32_t Renderer::GetWinHeight() const
    {
        return m_Window->GetHeight();
    }
}
