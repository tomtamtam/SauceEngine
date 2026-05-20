#include "ECS/ScriptableEntity.h"
#include <iostream>
#include <memory>

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

    void OnUpdate() override
    {
    }
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