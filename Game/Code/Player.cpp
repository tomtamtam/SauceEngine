#include "ECS/ScriptableEntity.h"
#include "ECS/Components.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <memory>

using namespace Sauce;

class Player : public Sauce::ScriptableEntity
{
public:
    void OnInit() override
    {
        std::cout << "Init Player\n";
    }

    void OnStart() override
    {
        std::cout << "Start Player\n";
    }

    void OnUpdate(float dt) override
    {

    }
private:
};

extern "C"
{
    std::shared_ptr<Sauce::ScriptableEntity> CreatePlayer()
    {
        return std::make_shared<Player>();
    }

    void DeletePlayer(Sauce::ScriptableEntity* script)
    {
        delete script;
    }
}
