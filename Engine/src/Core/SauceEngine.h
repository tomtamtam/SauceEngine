#pragma once

#include "Core/UUID.h"
#include <string>

namespace Sauce
{
    void SwitchScene(UUID uuid);
    void SwitchScene(const std::string &name);

    UUID AddEntity();
}
