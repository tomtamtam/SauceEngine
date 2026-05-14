#include "SauceEngine.h"
#include "Core/Application.h"
#include <memory>

namespace Sauce
{
    std::shared_ptr<Application> s_App = std::make_shared<Application>();

    void SwitchScene(UUID uuid)
    {
        s_App->SwitchScene(uuid);
    }

    void SwitchScene(const std::string &name)
    {
        s_App->SwitchScene(s_App->GetSceneByName(name));
    }

    UUID AddEntity()
    {
        return s_App->GetCurrentScene()->AddEntityUUID();
    }

}
