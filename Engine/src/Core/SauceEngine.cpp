#include "SauceEngine.h"
#include "Core/EntryPoint.h"

namespace Sauce
{
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
