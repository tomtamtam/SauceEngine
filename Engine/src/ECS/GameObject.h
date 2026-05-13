#pragma once

#include "Scene.h"

#include <cstdint>

namespace Sauce
{

  class GameObject
  {
  public:
    template<typename T>
    void AddComponent()
    {
        m_Scene->AttachComponent<T>(m_ID);
    }

    template<typename T>
    void RemoveComponent()
    {
        m_Scene->RemoveComponent<T>(m_ID);
    }

    template<typename T>
    T* GetComponent()
    {
        return m_Scene->GetComponent<T>(m_ID);
    }

    void Destroy()
    {
        m_Scene->DestroyEntity(m_ID);
    }

    virtual void Update(float deltaTime);
    virtual void Start();
  
  private:
    uint64_t m_ID;
    Scene *m_Scene;
  };
}
