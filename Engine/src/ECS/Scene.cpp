#include "Scene.h"
#include "Core/UUID.h"
#include "Render/Viewport.h"
#include "Scene/Entity.h"
#include <memory>
#include <string>

namespace Sauce
{
  Scene::Scene(const std::string &name)
    : m_ID(), m_Name(name)
  {
    m_Entities = {};
    m_Viewports = {};
  }

  Scene::Scene(const UUID &id)
    : m_ID(id), m_Name("Scene")
  {
    m_Entities = {};
    m_Viewports = {};
  }

  std::shared_ptr<Entity> Scene::GetEntityByID(UUID &&id)
  {
    return m_Entities.at(id);
  }

  std::shared_ptr<Viewport> Scene::GetViewportByID(UUID &&id)
  {
    return m_Viewports.at(id);
  }
}
