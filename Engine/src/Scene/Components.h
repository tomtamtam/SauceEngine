#pragma once

#include "Core/UUID.h"

namespace Sauce
{
  struct Component
  {
    virtual ~Component() = default;
  };


  struct IdComponent : Component
  {
  public:
    IdComponent() {};
    IdComponent(UUID id) : m_ID(id) {};

    UUID GetID() { return m_ID; }

  private:
    UUID m_ID;
  };
}
