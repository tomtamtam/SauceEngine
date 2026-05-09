#pragma once

#include "Core/UUID.h"
#include "Render/OrthographicCamera.h"
#include "glm/ext/vector_float2.hpp"
#include <cstdint>
#include <glm/glm.hpp>

namespace Sauce
{
  class Viewport
  {
  public:
    Viewport(OrthoGraphicCamera &&camera);
    void Draw();

  private:
    glm::vec2 m_Position;
    uint32_t m_Width, m_Height;
    UUID m_ID;
    OrthoGraphicCamera &&m_Camera;
  };
}
