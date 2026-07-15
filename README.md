# Sauce Engine
Is a C++ Game Engine using OpenGL

Releases will be pushed to the "main" branch, the current development is inside the private "dev" branch.

## Build
### Dependencies
git, cmake, make, opengl, X11 / wayland

### How to build
```bash
git clone https://github.com/tomtamtam/SauceEngine.git
cd SauceEngine
chmod +x setup.sh
./setup.sh
```
**NOTE: Right now compiling is only possible on Linux devices**

### Used Libraries
#### Inside repo (Engine/vendor):
glad, stb_image, imgui

#### Fetched through CMake:
glfw, glm, nlohman/json, lua, entt, assimp

## Use the Engine
Run
```bash
sauce-engine create projname
cd projname
sauce-engine edit .
```


