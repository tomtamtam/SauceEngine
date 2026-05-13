#pragma once

#include "Core/Application.h"
#include <memory>


int main(int argc, char** argv)
{
    auto app = std::make_shared<Sauce::Application>();
    app->Run();
}