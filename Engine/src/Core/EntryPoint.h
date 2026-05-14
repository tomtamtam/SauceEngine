#pragma once

#include "Core/Application.h"
#include <memory>

namespace Sauce
{
    auto s_App = std::make_shared<Sauce::Application>();
}

int main(int argc, char** argv)
{
    Sauce::s_App->Run();
}
