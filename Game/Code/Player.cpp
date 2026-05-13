#include <iostream>

extern "C" void Update(float deltaTime)
{

}

extern "C" void Start()
{
    std::cout << "start\n";
}


extern "C" void OnDestroy()
{
    std::cout << "on OnDestroy\n";
}
