#include "Core/UUID.h"
#include "ECS/Components.h"
#include "ECS/Entity.h"
#include <iostream>

using namespace Sauce;

class Player : public Entity
{
public:
    Player(UUID uuid)
    {
        ID = uuid;
    }
    ~Player()
    {}
    void Start()
    {
        std::cout << "Start Player (uuid) " << ID << "\n";
        if(GetComponent<Code>() == NULL)
        {
            std::cout << "NULL (code comp) \n";
        }
    }
    void Update()
    {
        std::cout << "Player Update\n";

    }
    void OnDestroy()
    {
        std::cout << "destroying player\n";
    }
};

extern "C" Sauce::Entity* GetPlayer(UUID uuid)
{
    return new Player(uuid);
}
