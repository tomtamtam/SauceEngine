#include "Core/UUID.h"
#include "Scene/Entity.h"

#include <unordered_map>

namespace Sauce
{
  class Scene
  {
  public:

  private:
    std::unordered_map<uint64_t, Entity> m_Entities;
  };
}
