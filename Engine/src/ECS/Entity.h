#pragma once

#include "Core/UUID.h"
#include "Scene/Components.h"

#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>


namespace Sauce
{

  class Entity
  {
  public:
    Entity(std::string name);

    Entity(UUID id);

    template<typename T, typename... Args>
    void AddComponent(Args&&... args);

    template<typename T>
    void RemoveComponent();

    template<typename T>
    bool HasComponent() const;

    template<typename T>
    std::shared_ptr<T> GetComponent() const;

    void Start();
    void Update(float deltaTime);


  private:
    virtual void UserStart() = 0;
    virtual void UserUpdate(float deltaTime) = 0;

    std::string m_Name;
    std::unordered_map<std::type_index, std::shared_ptr<Component>> m_Components;
  };
}
