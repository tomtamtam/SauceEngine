#include "SceneSerializer.h"
#include <string>

namespace Sauce
{
    SceneSerializer::SceneSerializer(std::shared_ptr<Scene> scene)
        : m_Scene(scene)
    {}

    SceneSerializer::~SceneSerializer() {}

    void SceneSerializer::Serialze(const std::string &path)
    {
        // Scene => JSON
    }

    void SceneSerializer::Deserialze(const std::string &path)
    {
        //JSON => Scene
    }
}
