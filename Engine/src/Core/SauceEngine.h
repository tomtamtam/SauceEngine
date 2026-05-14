#pragma once

#include "Core/Application.h"
#include "Core/UUID.h"
#include <memory>
#include <string>

namespace Sauce
{
    extern std::shared_ptr<Application> s_App;
    void SwitchScene(UUID uuid);
    void SwitchScene(const std::string &name);

    UUID AddEntity();
}
