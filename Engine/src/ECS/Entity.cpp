#include "Entity.h"
#include "Scene/Components.h"
#include <cassert>

namespace Sauce
{

  Entity::Entity(std::string name)
  {
    m_Name = name;
    m_Components = {};
    AddComponent<IdComponent>();
  }

  Entity::Entity(UUID id)
  {
    m_Name = "Entity1";
    m_Components = {};
 
    AddComponent<IdComponent>(id);
  }

  template<typename T, typename... Args>
  void Entity::AddComponent(Args&&... args)
  {
    static_assert(!std::is_base_of<T, Component>(), "Error, in AddComponent(): added type is not base of struct Component");
    if(!HasComponent<T>()) { assert("Error, in GetComponent(): demanded type doesn't exist in Components"); }
    auto ptr = std::make_shared<T>(std::forward<Args>(args)...);
    m_Components.at(typeid(T)) = ptr;
  }


  template<typename T>
  void Entity::RemoveComponent()
  {
    static_assert(!std::is_base_of<T, Component>(), "Error, in RemoveComponent(): demanded type is not base of struct Component");
    if(!HasComponent<T>()) { assert("Error, in GetComponent(): demanded type doesn't exist in Components"); }
    m_Components.erase(typeid(T));
  }

  template<typename T>
  std::shared_ptr<T> Entity::GetComponent() const
  {
    static_assert(!std::is_base_of<Component, T>(), "Error, in GetComponent(): demanded type is not base of struct Component");
    if(!HasComponent<T>()) { assert("Error, in GetComponent(): demanded type doesn't exist in Components"); }
    return m_Components.at(typeid(T));
  }

  template<typename T>
  bool Entity::HasComponent() const
  {
    return m_Components.find(typeid(T)) != nullptr;
  }

  void Entity::Start()
  {
    for(auto &c : m_Components)
    {
      c.second->Start();
    }
    UserStart();
  }

  void Entity::Update(float deltaTime)
  {
    for(auto &c : m_Components)
    {
      c.second->Update(deltaTime);
    }
    UserUpdate(deltaTime);
  }
}

