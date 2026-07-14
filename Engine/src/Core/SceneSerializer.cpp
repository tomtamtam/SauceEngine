#include "SceneSerializer.h"
#include "ECS/Components.h"
#include "ECS/Entity.h"
#include "Render/Mesh.h"
#include <cstdint>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;
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

    void attachComponent(json component, Entity *entity)
    {
        switch(component["id"].get<int>())
        {
            case SCRIPT_CIDX:
            {
                entity->AddComponent<Script>();
                entity->GetComponent<Script>().ScriptName = component["values"][0];
                break;
            }
            case CAMERA_CIDX:
            {
                entity->AddComponent<CameraComponent>();
                CameraComponent c;
                c.FOV = component["values"][0];
                c.Width = component["values"][1];
                c.Height = component["values"][2];
                c.IsMain = component["values"][3];
                c.IsAdjustable = component["values"][4];
                entity->GetComponent<CameraComponent>() = c;
                break;
            }
            case TRANSFORM_CIDX:
            {
                auto [px, py, pz, rx, ry, rz, sx, sy, sz] = component["values"].get<std::array<float, 9>>();
                entity->AddComponent<TransformComponent>();
                entity->GetComponent<TransformComponent>().Translation = {px, py, pz};
                entity->GetComponent<TransformComponent>().Rotation = {rx, ry, rz};
                entity->GetComponent<TransformComponent>().Scale = {sx, sy, sz};
                break;
            }
            case MESH_INSTANCE_CIDX:
            {
                entity->AddComponent<MeshInstance>();
                entity->GetComponent<MeshInstance>().IsVisible = true;
                entity->GetComponent<MeshInstance>().PMesh = CreateCubeMesh();
                break;
            }
            default:
                break;
        }
    }

    void SceneSerializer::Deserialze(const std::string &path)
    {
        std::ifstream f(path);
        json jScene;
        f >> jScene;

        m_Scene->m_Name = jScene["name"];
        m_Scene->m_ID = (UUID) jScene["id"].get<uint64_t>();

        for(auto o : jScene["objects"])
        {
            Entity e(m_Scene->CreateEntity(o["name"], o["id"].get<uint64_t>()));
            for(auto c : o["components"])
            {
                attachComponent(c, &e);
            }
        }
    }
}
