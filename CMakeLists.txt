cmake_minimum_required(VERSION 4.0)

project(SauceEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# GLAD
add_library(glad STATIC
  Engine/vendor/glad/src/glad.c
)
target_include_directories(glad PUBLIC Engine/vendor/glad/include)

# GLFW
include(FetchContent)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4
)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# GLM
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

# EXECUTABLES
add_executable(SauceEngine
  Engine/src/Main.cpp
  Engine/src/Core/UUID.h Engine/src/Core/UUID.cpp
  Engine/src/ECS/Components.h
  Engine/src/ECS/Scene.h Engine/src/ECS/Scene.cpp
  Engine/src/ECS/Entity.h Engine/src/ECS/Entity.cpp
  Engine/src/Render/Window.h Engine/src/Render/Window.cpp
  Engine/src/Core/Application.h Engine/src/Core/Application.cpp
  Engine/src/Render/Renderer.h Engine/src/Render/Renderer.cpp
  Engine/src/Render/Viewport.h Engine/src/Render/Viewport.cpp
  Engine/src/Render/OrthographicCamera.h Engine/src/Render/OrthographicCamera.cpp
  Game/src/Scenes/TestScene.h Game/src/Scenes/TestScene.cpp
  Engine/src/ECS/ECS.h Engine/src/ECS/ECS.cpp 
)

target_include_directories(SauceEngine PRIVATE
  ${CMAKE_SOURCE_DIR}/Engine/src
  ${CMAKE_SOURCE_DIR}/Game/src
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  glad
  glfw
  glm::glm
)

# OPENGL
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE opengl32)
elseif(APPLE)
    find_library(OPENGL_LIBRARY OpenGL REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARY})
else()  # Linux
    find_package(OpenGL REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)
endif()
