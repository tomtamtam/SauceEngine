#pragma once

#include "Render/Texture.h"
#include <cstdint>

namespace Sauce
{
    class FrameBuffer
    {
    public:
        FrameBuffer(uint32_t width, uint32_t height);
        ~FrameBuffer();

        void Resize(uint32_t width, uint32_t height);
        void Bind();
        void Unbind();
        void Delete();
        inline uint32_t GetWidth() const { return m_Width; }
        inline uint32_t GetHeight() const { return m_Height; }
        inline uint32_t GetColorAttachment() const {return m_Texture.GetID();}
    private:
        void Create();

        uint32_t m_Fbo;
        uint32_t m_Rbo;
        Texture m_Texture;
        uint32_t m_Width, m_Height;
    };
}
