#include "Framebuffer.h"
#include "Render/Buffers/VertexBuffer.h"
#include <cstdint>
#include <iostream>

namespace Sauce
{
    FrameBuffer::FrameBuffer(uint32_t width, uint32_t height)
        : m_Texture(width, height), m_Width(width), m_Height(height)
    {
        Create();
    }

    FrameBuffer::~FrameBuffer()
    {
        Delete();
    }

    void FrameBuffer::Create()
    {
        glGenFramebuffers(1, &m_Fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, m_Fbo);

        m_Texture.Bind();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture.GetID(), 0);

        glGenRenderbuffers(1, &m_Rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, m_Rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_Rbo);

        auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if(fboStatus != GL_FRAMEBUFFER_COMPLETE)
            std::cerr << "ERROR in FrameBuffer: " << fboStatus << "\n";

        m_Texture.Unbind();

    }

    void FrameBuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_Fbo);
    }

    void FrameBuffer::Unbind()
    {
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::Delete()
    {
        Unbind();

        glDeleteRenderbuffers(1, &m_Rbo);
        glDeleteFramebuffers(1, &m_Fbo);
    }

    void FrameBuffer::Resize(uint32_t width, uint32_t height)
    {
        m_Width = width;
        m_Height = height;

        glBindFramebuffer(GL_FRAMEBUFFER, m_Fbo);

        m_Texture.Resize(width, height);
        m_Texture.Bind();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture.GetID(), 0);

        glBindRenderbuffer(GL_RENDERBUFFER, m_Rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_Rbo);

        auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if(fboStatus != GL_FRAMEBUFFER_COMPLETE)
            std::cerr << "ERROR in FrameBuffer: " << fboStatus << "\n";

        m_Texture.Unbind();
        Unbind();
    }
}
