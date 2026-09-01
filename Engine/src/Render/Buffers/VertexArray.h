#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace Sauce
{

    struct VertexArray
    {
        VertexArray();
        virtual ~VertexArray();

        void AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout);

        void Bind() const;
        void Unbind() const;

    private:
        uint32_t m_VAO;
    };
}
