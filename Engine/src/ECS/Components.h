#pragma once

#include "Core/UUID.h"
#include <cstdint>
#include <iostream>

namespace Sauce
{
  struct Component
  {
    virtual ~Component() = default;
    virtual void Start() = 0;
    virtual void Update(float deltaTime) = 0;
  };


  struct IdComponent : Component
  {
  public:
    IdComponent() {};
    IdComponent(UUID id) : m_ID(id) {};

    void Start()
    {
      std::cout << "start IdComponent: " << (uint64_t)m_ID << '\n';
    }

    void Update(float deltaTime)
    {

    }

    UUID GetID() { return m_ID; }

  private:
    UUID m_ID;
  };
}
