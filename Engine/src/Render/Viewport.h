#pragma once

#include "Render/OrthographicCamera.h"
#include <cstdint>
namespace Sauce
{
  class Viewport
  {
  public:
    void Draw(OrthoGraphicCamera camera);

  private:
    uint32_t m_Width, m_Height;
  };
}
