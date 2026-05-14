# Sauce Engine
Is a C++ Game Engine using OpenGL

Releases will be pushed to the "main" branch, the current development is inside the private "dev" branch.

## Build
### Dependencies
Inside repo (Engine/vendor):
- glad
- stb_image
Fetched through CMake:
- glfw
- glm
- nlohman/json
- lua
- (opengl)

### How to build
git clone https://github.com/tomtamtam/SauceEngine.git
cd SauceEngine
mkdir build && cd build
cmake ..
make
**NOTE: Right now compiling is only possible on Linux devices**
