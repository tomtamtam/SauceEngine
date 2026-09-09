#pragma once

#include <memory>

#include "Core/Application.h"

inline void Run(int argc, char** argv)
{
    std::shared_ptr<Sauce::Application> app = std::make_shared<Sauce::Application>();
    app->Run();
}
