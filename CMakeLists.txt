cmake_minimum_required(VERSION 3.20)
project(SauceEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#GLAD
add_library(glad STATIC
    Engine/vendor/glad/src/glad.c
)
target_include_directories(glad PUBLIC Engine/vendor/glad/include)
set_target_properties(glad PROPERTIES POSITION_INDEPENDENT_CODE ON)

#GLFW
include(FetchContent)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

#GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.1
)
FetchContent_MakeAvailable(glm)

# JSON
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

# LUA
FetchContent_Declare(
    lua
    GIT_REPOSITORY "https://github.com/marovira/lua"
    GIT_TAG 5.4.8
  )

FetchContent_MakeAvailable(lua)

# ENTT
FetchContent_Declare(
    entt
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG        v3.14.0
)
FetchContent_MakeAvailable(entt)

# STB IMAGE
add_library(stb_image STATIC
  Engine/vendor/stb_image/stb_image.cpp
)
target_include_directories(stb_image PUBLIC Engine/vendor/stb_image)
set_target_properties(stb_image PROPERTIES POSITION_INDEPENDENT_CODE ON)

#ENGINE (Static Library)
add_library(SauceEngine SHARED
    Engine/src/Core/UUID.cpp
    Engine/src/Core/Application.cpp
    Engine/src/Render/Window.cpp
    Engine/src/Render/Renderer.cpp
    Engine/src/Render/Viewport.cpp
    Engine/src/Render/Buffers/IndexBuffer.cpp
    Engine/src/Render/Buffers/VertexArray.cpp
    Engine/src/Render/Buffers/VertexBuffer.cpp
    Engine/src/Render/Texture.cpp
    Engine/src/Render/Shader.cpp
    Engine/src/ECS/Scene.cpp
    Engine/src/ECS/Entity.cpp
    Engine/src/ECS/ScriptableEntity.cpp
    Engine/src/Core/ScriptEngine.cpp
)

target_include_directories(SauceEngine PUBLIC
    ${CMAKE_SOURCE_DIR}/Engine/src
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(SauceEngine PUBLIC
    glad
    glfw
    glm::glm
    nlohmann_json::nlohmann_json
    stb_image
    lua::lua
    EnTT::EnTT
)

#OpenGL
if(WIN32)
    target_link_libraries(SauceEngine PUBLIC opengl32)
elseif(APPLE)
    find_library(OPENGL_LIBRARY OpenGL REQUIRED)
    target_link_libraries(SauceEngine PUBLIC ${OPENGL_LIBRARY})
else()
    find_package(OpenGL REQUIRED)
    target_link_libraries(SauceEngine PUBLIC OpenGL::GL)
endif()

#GAME (Shared Library / Plugin)
add_library(Game SHARED
    Game/Code/Player.cpp
)
target_include_directories(Game PUBLIC
    ${CMAKE_SOURCE_DIR}/Game/Code
)
target_link_libraries(Game PUBLIC
    SauceEngine
    EnTT::EnTT
)

set_target_properties(Game PROPERTIES
    BUILD_RPATH "$ORIGIN"
)

#App (Executable)

add_executable(App
    Game/Code/Main.cpp
)
target_include_directories(App PRIVATE
    ${CMAKE_SOURCE_DIR}/Game/Code
)
target_link_libraries(App PRIVATE 
  SauceEngine
)


set_target_properties(Game PROPERTIES
    BUILD_RPATH "$ORIGIN"
)

set_target_properties(App PROPERTIES
    BUILD_RPATH "$ORIGIN"
)
