#include "ECS/Entity.h"
#include <iostream>

class Player : Sauce::Entity
{
    void Start() override
    {
        std::cout << "test\n";
    }
    void Update() override
    {

    }
    void OnDestroy() override
    {

    }
};
