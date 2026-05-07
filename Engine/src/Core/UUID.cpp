#include "UUID.h"

#include <cstddef>
#include <cstdint>
#include <random>

namespace Sauce
{

  static std::random_device s_RandomDevice;
  static std::mt19937_64 s_Engine(s_RandomDevice());
  static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

  UUID::UUID()
    : m_UUID(s_UniformDistribution(s_Engine))
  {

  }

  UUID::UUID(uint64_t uuid)
  {

  }

}

namespace std {
  
  template<typename T> struct hash;

  template<>
    struct hash<Sauce::UUID>
    {
      std::size_t operator()(const Sauce::UUID &uuid) const
      {
        return (uint64_t) uuid;
      }
    };
}
