# Sauce Engine
Is a C++ Game Engine using OpenGL

Releases will be pushed to the "main" branch, the current development is inside the private "dev" branch.

## Build
### Dependencies
git, cmake, make, opengl, X11 / wayland

### How to build & install
```bash
git clone https://github.com/tomtamtam/SauceEngine.git
cd SauceEngine
mkdir build
cmake --build build
sudo cmake --install build --prefix /usr
```
**NOTE: Right now compiling is only possible on Linux devices**

### Used Libraries
#### Inside repo (Engine/vendor):
glad, stb_image, imgui, nativefiledialog

#### Fetched through CMake:
glfw, glm, nlohman/json, lua, entt, assimp

## Use the Engine
Run
```bash
# open the ui-editor to create / open a project
sauce-editor
# to directly open a existing project
sauce-editor <path-to-project-folder>
```


