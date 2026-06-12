#include "Application.h"
#include <array>
#include <cassert>
#include <cstdint>
#include <dlfcn.h>
#include <iostream>
#include <memory>
#include <string>
#include "Core/ScriptEngine.h"
#include "ECS/Components.h"
#include "ECS/Scene.h"
#include "GLFW/glfw3.h"
#include "Render/Buffers/IndexBuffer.h"
#include "Render/Buffers/VertexArray.h"
#include "Render/Buffers/VertexBufferLayout.h"
#include "Render/Renderer.h"
#include "Render/Shader.h"
#include "Util/Json.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "nlohmann/json.hpp"
#include "ECS/Entity.h"


namespace Sauce
{
    Application::Application()
        : m_IsRunning(true), m_Renderer(nullptr)
    {
    }

    Application::~Application()
    {
    }


    void Application::Setup()
    {
        auto self = shared_from_this();
        auto onShouldClose = [self](){self->Terminate();};

        m_Renderer = std::make_unique<Renderer>(onShouldClose);
        m_Renderer->CreateWindow("Game", 800, 800);
        std::cout << "after renderer\n";

        m_CurrentScene = LoadScene("../Game/Scenes/TestScene.json");
        m_CurrentScene->Init();
        std::cout << "after Scene\n";

        auto shader = std::make_shared<Shader>("../Game/Shaders/Default.glsl");
        m_Renderer->Set<ShaderType>(shader);
    }

    void Application::Terminate()
    {
        m_IsRunning = false;
    }


    void Application::Run()
    {
        std::cout << "Run\n";
        Setup();
        std::cout << "after Setup()\n";

        m_BeginTime = glfwGetTime();
        m_EndTime = glfwGetTime();
        m_DT = -1.0f;

        m_CurrentScene->Start();

        float vertices[] = 
        {
            -0.5f, -0.5f, 0.0f,  // links unten
             0.5f, -0.5f, 0.0f,  // rechts unten
             0.0f,  0.5f, 0.0f   // oben mitte
        };
 
        VertexBuffer vertexBuffer(vertices, sizeof(vertices));
 
        uint32_t indices[] =
        {
            0, 1, 2
        };
 
        IndexBuffer indexBuffer(indices, sizeof(indices));
 
        VertexBufferLayout bufferLayout;
        bufferLayout.Push<float>(3);
 
        VertexArray vertexArray;
        vertexArray.AddBuffer(vertexBuffer, bufferLayout);

        while (m_IsRunning)
        {
            m_Renderer->BeginDraw();
            vertexBuffer.Bind();
            indexBuffer.Bind();
            glDrawElements(GL_TRIANGLES, indexBuffer.getCount(),GL_UNSIGNED_INT, 0);
            m_Renderer->Set<RenderType>(BATCH);
            m_Renderer->Draw();

            if(m_DT >= 0.0f)
            {
                m_CurrentScene->Update();
            }

            m_EndTime = glfwGetTime();
            m_DT = m_EndTime - m_BeginTime;
            m_BeginTime = m_EndTime;
        }

        m_Renderer->KillWindow();
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

    Script LoadScriptFromJson(json j)
    {
        std::string name = j["values"][0];
        Script t = ScriptEngine::AttachScript(ScriptEngine::LoadScript(name, "./libGame.so"), name);
        return t;
    }




    std::shared_ptr<Scene> Application::LoadScene(const std::string &path)
    {
        json j = ParseJsonFromFile(path);
        std::string name = j["name"];
        UUID uuid = (UUID)j["id"].get<uint64_t>();

        auto scene = std::make_shared<Scene>(uuid, name);


        //objects
        uint32_t nObjects = j["objects"].size();


        for(int i = 0; i < nObjects; i++)
        {
            json obj = j["objects"][i];
            std::string objName = obj["name"];
            UUID objUUID = (UUID)obj["id"].get<uint64_t>();

            Entity ent(scene->CreateEntity(objName, objUUID));


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
                    ent.AddComponent<Script>();
                    ent.GetComponent<Script>() = LoadScriptFromJson(component);
                    break;
                }
            }
        }
        return scene;
    }
}
