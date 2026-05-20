#include "Application.h"
#include <array>
#include <cassert>
#include <cstdint>
#include <dlfcn.h>
#include <memory>
#include <string>
#include "ECS/Components.h"
#include "ECS/Scene.h"
#include "GLFW/glfw3.h"
#include "Render/Shader.h"
#include "Util/Json.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "nlohmann/json.hpp"
#include "ECS/Entity.h"

using json = nlohmann::json;

namespace Sauce
{
    Application::Application()
        : m_IsRunning(true)
    {
    }

    Application::~Application()
    {
    }


    void Application::Setup()
    {
        auto self = shared_from_this();
        auto onShouldClose = [self](){self->Terminate();};
        m_CurrentWindow = {"test", 800, 800, onShouldClose};
        m_CurrentWindow.Init();


        Shader shader("../Game/Shaders/Default.glsl");
    }

    void Application::Terminate()
    {
        m_IsRunning = false;
    }


    void Application::Run()
    {
        Setup();

        m_BeginTime = glfwGetTime();
        m_EndTime = glfwGetTime();
        m_DT = -1.0f;

        while (m_IsRunning)
        {
            m_CurrentWindow.Update();

            if(m_DT >= 0.0f)
            {
            }

            m_EndTime = glfwGetTime();
            m_DT = m_EndTime - m_BeginTime;
            m_BeginTime = m_EndTime;
        }

        m_CurrentWindow.Destroy();
    }

    glm::mat4 GetTransformFromJson(json component)
    {
        json values = component["values"];
        glm::mat4 transform = glm::mat4(1.0f);

        auto [px, py, pz, rx, ry, rz, sx, sy, sz] = values.get<std::array<float, 9>>();

        //position
        transform = glm::translate(transform, glm::vec3(px, py, pz));
        
        //rotation
        transform = glm::rotate(transform, glm::radians(rx), glm::vec3(1, 0, 0));
        transform = glm::rotate(transform, glm::radians(ry), glm::vec3(0, 1, 0));
        transform = glm::rotate(transform, glm::radians(rz), glm::vec3(0, 0, 1));

        //scale
        transform = glm::scale(transform, glm::vec3(sx, sy, sz));

        return transform;
    }

    Code GetCodeByName(const std::string &className)
    {
        void *handle = dlopen("./libGame.so", RTLD_NOW); 

        if (!handle)
        {
            throw std::runtime_error(dlerror());
        }

        dlerror();

        using GetFunc = Entity(*)();

        std::string name = "Get" + className;
        GetFunc getEnt = reinterpret_cast<GetFunc>(dlsym(handle, name.c_str()));

        Entity e = getEnt();

        const char* err = dlerror();
        if (err)
        {
            dlclose(handle);
            throw std::runtime_error(err);
        }

        Code c(e);

    }

    std::shared_ptr<Scene> Application::LoadScene(const std::string &path)
    {
        json j = ParseJsonFromFile(path);
        std::string name = j["name"];
        UUID uuid = j["id"].get<uint64_t>();

        auto scene = std::make_shared<Scene>(uuid, name);

        //objects
        uint32_t nObjects = j["objects"].size();

        for(int i = 0; i < nObjects; i++)
        {
            json obj = j["objects"][i];
            std::string objName = obj["name"];
            UUID objUUID = obj["id"].get<uint64_t>();

            Entity ent(scene->CreateEntity(name, uuid));

            //components
            uint32_t nComponents = obj["components"].size();

            for(int j = 0; j < nComponents; j++)
            {
                json component = obj["components"][j];
                uint32_t id = component["id"];

                switch (id)
                {
                case 1:
                    ent.AddComponent<TransformComponent>();
                    ent.GetComponent<TransformComponent>() = GetTransformFromJson(component);
                    break;
                case 0:
                    ent.AddComponent<Code>(ent);
                    break;
                }
            }
        }
    }
}
