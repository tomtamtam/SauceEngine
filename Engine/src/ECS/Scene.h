#include "Core/UUID.h"
#include "Render/Viewport.h"
#include "Scene/Entity.h"

#include <memory>
#include <string>
#include <unordered_map>

namespace Sauce
{
  class Scene
  {
  public:
    Scene(const std::string &name);
    Scene(const UUID &id);
    std::shared_ptr<Entity> GetEntityByID(UUID &&id);
    std::shared_ptr<Viewport> GetViewportByID(UUID &&id);

    void Start();
    void Update(float deltaTime);

    virtual void UserStart() = 0;
    virtual void UserUpdate(float deltaTime) = 0;

  private:
    UUID m_ID;
    std::string m_Name;
    std::unordered_map<uint64_t, std::shared_ptr<Entity>> m_Entities;
    std::unordered_map<uint64_t, std::shared_ptr<Viewport>> m_Viewports;
  };
}
