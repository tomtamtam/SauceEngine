#include "Render/Buffers/VertexBufferLayout.h"
#include <iostream>

extern "C" void Update(float deltaTime)
{
    std::cout << "Update\n";
}

extern "C" void Start()
{
    VertexBufferLayout vbl;
    std::cout << "start\n";
}


extern "C" void OnDestroy()
{
    std::cout << "on OnDestroy\n";
}
