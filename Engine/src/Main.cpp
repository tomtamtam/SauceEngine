#include "Core/Application.h"
#include <memory>

int main()
{
  auto app = std::make_shared<Sauce::Application>();
  app->Run();
}