#pragma once

#include <cstdint>
#include <glad/glad.h>


struct VertexBuffer
{
  VertexBuffer(void* data, uint32_t size);

  virtual ~VertexBuffer();

  void Bind() const;
  void Unbind() const;
  
private:
  uint32_t m_BufferID;
};
