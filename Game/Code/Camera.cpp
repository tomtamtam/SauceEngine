#include "ECS/ScriptableEntity.h"
#include "ECS/Components.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <memory>

using namespace Sauce;


class Camera : public Sauce::ScriptableEntity
{
public:
    void OnInit() override
    {
        std::cout << "Init Camera\n";
    }

    void OnStart() override
    {
        std::cout << "Start Camera\n";
    }

    void OnUpdate(float dt) override
    {
        if(Input->GetKeyJustPressed(GLFW_KEY_W))
        {
            GetComponent<TransformComponent>().Translation.z += 0.1 * dt;
        }
        if(Input->GetKeyJustPressed(GLFW_KEY_S))
        {
            GetComponent<TransformComponent>().Translation.z -= 0.1 * dt;
        }
        if(Input->GetKeyJustPressed(GLFW_KEY_D))
        {
            GetComponent<TransformComponent>().Translation.x += 0.1 * dt;
        }
        if(Input->GetKeyJustPressed(GLFW_KEY_A))
        {
            GetComponent<TransformComponent>().Translation.x -= 0.1 * dt;
        }
        if(Input->GetKeyJustPressed(GLFW_KEY_LEFT))
        {
            GetComponent<TransformComponent>().Rotation.x += 0.1 * dt;
        }
        if(Input->GetKeyJustPressed(GLFW_KEY_RIGHT))
        {
            GetComponent<TransformComponent>().Rotation.x -= 0.1 * dt;
        }
    }
private:
};

extern "C"
{
    std::shared_ptr<Sauce::ScriptableEntity> CreateCamera()
    {
        return std::make_shared<Camera>();
    }

    void DeleteCamera(Sauce::ScriptableEntity* script)
    {
        delete script;
    }
}
