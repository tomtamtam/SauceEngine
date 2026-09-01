#include "SceneSerializer.h"
#include "ECS/Components.h"
#include "ECS/Entity.h"
#include "Render/Mesh.h"
#include "entt/entity/fwd.hpp"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;
namespace Sauce
{
    SceneSerializer::SceneSerializer(std::shared_ptr<Scene> scene)
        : m_Scene(scene)
    {}

    SceneSerializer::~SceneSerializer() {}

    void SceneSerializer::Serialze(const std::string &path) // SAVE
    {
        json jScene;
        jScene["name"] = m_Scene->m_Name;
        jScene["id"] = (uint64_t)m_Scene->m_ID;

        auto view = m_Scene->m_Registry.view<UUIDComponent>();

        int i = 0;
        for(auto e : view)
        {
        int compCount = 0;
            //UUID
            if(auto *uuid = m_Scene->m_Registry.try_get<UUIDComponent>(e))
            {
                json jUUID;
                jUUID["id"] = UUID_CIDX;
                jUUID["values"][0] = (uint64_t)uuid->Id;
                jUUID["values"][1] = uuid->Name;
                jScene["objects"][i]["components"][compCount] = jUUID;
                compCount ++;
            }

            //Transform
            if(auto *tc = m_Scene->m_Registry.try_get<TransformComponent>(e))
            {
                json jTransform;
                jTransform["id"] = TRANSFORM_CIDX;
                jTransform["values"][0] = tc->Translation.x;
                jTransform["values"][1] = tc->Translation.y;
                jTransform["values"][2] = tc->Translation.z;

                jTransform["values"][3] = tc->Rotation.x;
                jTransform["values"][4] = tc->Rotation.y;
                jTransform["values"][5] = tc->Rotation.z;

                jTransform["values"][6] = tc->Scale.x;
                jTransform["values"][7] = tc->Scale.y;
                jTransform["values"][8] = tc->Scale.z;

                jScene["objects"][i]["components"][compCount] = jTransform;
                compCount ++;
            }

            //Script
            if(auto *sc = m_Scene->m_Registry.try_get<Script>(e))
            {
                json jScript;
                jScript["id"] = SCRIPT_CIDX;
                jScript["values"][0] = sc->ScriptName;
                jScene["objects"][i]["components"][compCount] = jScript;
                compCount ++;
            }

            //Camera
            if(auto *cc = m_Scene->m_Registry.try_get<CameraComponent>(e))
            {
                json jCamera;
                jCamera["id"] = CAMERA_CIDX;
                jCamera["values"][0] = cc->FOV;
                jCamera["values"][1] = cc->Width;
                jCamera["values"][2] = cc->Height;
                jCamera["values"][3] = cc->IsMain;
                jCamera["values"][4] = cc->IsAdjustable;

                jScene["objects"][i]["components"][compCount] = jCamera;
                compCount ++;
            }

            //MeshInstance
            if(auto *mc = m_Scene->m_Registry.try_get<MeshInstance>(e))
            {
                json jMesh;
                jMesh["id"] = MESH_INSTANCE_CIDX;
                jMesh["values"][0] = mc->IsVisible;
                jScene["objects"][i]["components"][compCount] = jMesh;
                compCount ++;
            }

            i++;
        }
        std::ofstream f(path);
        if(!f.is_open())
        {
            std::cerr << "failed to open file: " << path << '\n';
            return;
        }
        f << jScene;
        f.close();
        std::cout << "wrote to file\n";
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
            case UUID_CIDX:
            {
                entity->AddComponent<UUIDComponent>();
                entity->GetComponent<UUIDComponent>().Id = (UUID)component["values"][0].get<uint64_t>();
                entity->GetComponent<UUIDComponent>().Name = component["values"][1].get<std::string>();
            }
            default:
                break;
        }
    }

    void SceneSerializer::Deserialze(const std::string &path) // LOAD
    {
        std::ifstream f(path);
        json jScene;
        f >> jScene;

        m_Scene->m_Name = jScene["name"];
        m_Scene->m_ID = (UUID) jScene["id"].get<uint64_t>();

        for(auto o : jScene["objects"])
        {
            Entity e(m_Scene->CreateEntity());
            for(auto c : o["components"])
            {
                attachComponent(c, &e);
            }
        }
    }
}
