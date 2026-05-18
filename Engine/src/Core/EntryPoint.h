#pragma once

#include "Core/Application.h"
#include <iostream>
#include <memory>

int main(int argc, char** argv)
{
    std::shared_ptr<Sauce::Application> app = std::make_shared<Sauce::Application>();
    app->Run();
}
