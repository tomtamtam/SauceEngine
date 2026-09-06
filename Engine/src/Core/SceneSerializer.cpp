// noincludeformat

#include <cstdint>
#include <nlohmann/json.hpp>

#include <fstream>
#include <iostream>
#include <string>

#include "SceneSerializer.h"

#include "Core/AssetSystem.h"

#include "ECS/Components.h"
#include "ECS/Entity.h"

#include "entt/entity/fwd.hpp"

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
                jUUID["values"][VIDX::CUUID::ID] = (uint64_t)uuid->Id;
                jUUID["values"][VIDX::CUUID::NAME] = uuid->Name;
                jScene["objects"][i]["components"][compCount] = jUUID;
                compCount ++;
            }

            //Transform
            if(auto *tc = m_Scene->m_Registry.try_get<TransformComponent>(e))
            {
                json jTransform;
                jTransform["id"] = TRANSFORM_CIDX;
                jTransform["values"][VIDX::CTransform::TRANSFORM_X] = tc->Translation.x;
                jTransform["values"][VIDX::CTransform::TRANSFORM_Y] = tc->Translation.y;
                jTransform["values"][VIDX::CTransform::TRANSFORM_Z] = tc->Translation.z;

                jTransform["values"][VIDX::CTransform::ROTATION_X] = tc->Rotation.x;
                jTransform["values"][VIDX::CTransform::ROTATION_Y] = tc->Rotation.y;
                jTransform["values"][VIDX::CTransform::ROTATION_Z] = tc->Rotation.z;

                jTransform["values"][VIDX::CTransform::SCALE_X] = tc->Scale.x;
                jTransform["values"][VIDX::CTransform::SCALE_Y] = tc->Scale.y;
                jTransform["values"][VIDX::CTransform::SCALE_Z] = tc->Scale.z;

                jScene["objects"][i]["components"][compCount] = jTransform;
                compCount ++;
            }

            //Script
            if(auto *sc = m_Scene->m_Registry.try_get<Script>(e))
            {
                json jScript;
                jScript["id"] = SCRIPT_CIDX;
                jScript["values"][VIDX::CScript::NAME] = sc->ScriptName;
                jScene["objects"][i]["components"][compCount] = jScript;
                compCount ++;
            }

            //Camera
            if(auto *cc = m_Scene->m_Registry.try_get<CameraComponent>(e))
            {
                json jCamera;
                jCamera["id"] = CAMERA_CIDX;
                jCamera["values"][VIDX::CCamera::FOV] = cc->FOV;
                jCamera["values"][VIDX::CCamera::WIDHT] = cc->Width;
                jCamera["values"][VIDX::CCamera::HEIGHT] = cc->Height;
                jCamera["values"][VIDX::CCamera::IS_MAIN] = cc->IsMain;
                jCamera["values"][VIDX::CCamera::IS_ADJUSTABLE] = cc->IsAdjustable;

                jScene["objects"][i]["components"][compCount] = jCamera;
                compCount ++;
            }

            //MeshInstance
            if(auto *mc = m_Scene->m_Registry.try_get<MeshInstance>(e))
            {
                json jMesh;
                jMesh["id"] = MESH_INSTANCE_CIDX;
                jMesh["values"][VIDX::CMesh::ID] = mc->IsVisible;
                jScene["objects"][i]["components"][compCount] = jMesh;
                compCount ++;
            }
            
            //Texture
            if(auto *tc = m_Scene->m_Registry.try_get<TextureComponent>(e))
            {
            	json jTexture;
            	jTexture["id"] = TEXTURE_CIDX;
            	jTexture["values"][VIDX::CTexture::ID] = tc->textureId;
            	jScene["objects"][i]["components"][compCount] = jTexture;
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

    void SceneSerializer::AttachComponent(json component, Entity *entity)
    {
        switch(component["id"].get<int>())
        {
            case SCRIPT_CIDX:
            {
                entity->AddComponent<Script>();
                entity->GetComponent<Script>().ScriptName = component["values"][VIDX::CScript::NAME];
                break;
            }
            case CAMERA_CIDX:
            {
                entity->AddComponent<CameraComponent>();
                CameraComponent c;
                c.FOV = component["values"][VIDX::CCamera::FOV];
                c.Width = component["values"][VIDX::CCamera::WIDHT];
                c.Height = component["values"][VIDX::CCamera::HEIGHT];
                c.IsMain = component["values"][VIDX::CCamera::IS_MAIN];
                c.IsAdjustable = component["values"][VIDX::CCamera::IS_ADJUSTABLE];
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
                entity->GetComponent<MeshInstance>().IsVisible = component["values"][VIDX::CMesh::IS_VISIBLE];
                UUID meshId {component["values"][VIDX::CMesh::ID]};
                entity->GetComponent<MeshInstance>().meshId = meshId;
                break;
            }
            case UUID_CIDX:
            {
                entity->AddComponent<UUIDComponent>();
                entity->GetComponent<UUIDComponent>().Id = (UUID)component["values"][VIDX::CUUID::ID].get<uint64_t>();
                entity->GetComponent<UUIDComponent>().Name = component["values"][VIDX::CUUID::NAME].get<std::string>();
            }
            case TEXTURE_CIDX:
            {
            	entity->AddComponent<TextureComponent>();
            	entity->GetComponent<TextureComponent>().textureId = component["values"][VIDX::CTexture::ID].get<uint32_t>();
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
                AttachComponent(c, &e);
            }
        }
    }
}
