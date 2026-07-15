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

#IMGUI
add_library(imgui STATIC
  ${CMAKE_SOURCE_DIR}/Engine/vendor/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/Engine/vendor/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/Engine/vendor/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/Engine/vendor/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/Engine/vendor/imgui/imgui_demo.cpp
    ${CMAKE_SOURCE_DIR}/Engine/vendor/imgui/backends/imgui_impl_glfw.cpp
    ${CMAKE_SOURCE_DIR}/Engine/vendor/imgui/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC
  ${CMAKE_SOURCE_DIR}/Engine/vendor/imgui
    ${CMAKE_SOURCE_DIR}/Engine/vendor/imgui/backends
)
target_link_libraries(imgui PUBLIC glfw glad)

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
    GIT_TAG        v3.16.0
)
FetchContent_MakeAvailable(entt)

# ASSIMP
# FetchContent_Declare(
#     assimp
#     GIT_REPOSITORY https://github.com/assimp/assimp.git
#     GIT_TAG        v5.4.3        # oder eine andere Version/Branch
# )
# 
# set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
# set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
# set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
# set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
# set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)
# 
# FetchContent_MakeAvailable(assimp)
# 
# STB IMAGE
add_library(stb_image STATIC
  Engine/vendor/stb_image/stb_image.cpp
)
target_include_directories(stb_image PUBLIC Engine/vendor/stb_image)
set_target_properties(stb_image PROPERTIES POSITION_INDEPENDENT_CODE ON)

# nativefiledialog
set(NATIVEFILEDIALOG_DIR "${CMAKE_SOURCE_DIR}/Engine/vendor/nativefiledialog")
add_library(nativefiledialog STATIC
    "${NATIVEFILEDIALOG_DIR}/nfd_common.c"
)
target_include_directories(nativefiledialog
    PUBLIC
        "${NATIVEFILEDIALOG_DIR}/include"
        "${NATIVEFILEDIALOG_DIR}"
)
if(WIN32)
    target_sources(nativefiledialog PRIVATE
        "${NATIVEFILEDIALOG_DIR}/nfd_win.cpp"
    )
elseif(APPLE)
    target_sources(nativefiledialog PRIVATE
        "${NATIVEFILEDIALOG_DIR}/nfd_cocoa.m"
    )
    target_link_libraries(nativefiledialog PRIVATE "-framework Cocoa")
else() # Linux/Unix - using GTK backend
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
    target_sources(nativefiledialog PRIVATE
        "${NATIVEFILEDIALOG_DIR}/nfd_gtk.c"
    )
    target_include_directories(nativefiledialog PRIVATE ${GTK3_INCLUDE_DIRS})
    target_link_libraries(nativefiledialog PRIVATE ${GTK3_LIBRARIES})
    target_compile_options(nativefiledialog PRIVATE ${GTK3_CFLAGS_OTHER})
endif()
set_target_properties(nativefiledialog PROPERTIES
    C_STANDARD 11
)

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
    Engine/src/Core/ScriptEngine.cpp
    Engine/src/Core/SceneSerializer.cpp
    Engine/src/Input/KeyboardListener.cpp
    Engine/src/Input/InputSystem.cpp
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
    # assimp
)

#OpenGL
add_library(sauce_opengl INTERFACE)
if(WIN32)
    target_link_libraries(sauce_opengl INTERFACE opengl32)
elseif(APPLE)
    find_library(OPENGL_LIBRARY OpenGL REQUIRED)
    target_link_libraries(sauce_opengl INTERFACE ${OPENGL_LIBRARY})
else()
    find_package(OpenGL REQUIRED)
    target_link_libraries(sauce_opengl INTERFACE OpenGL::GL)
endif()

target_link_libraries(SauceEngine PUBLIC sauce_opengl)

# Terminal App

add_executable(sauce-engine
    ${CMAKE_SOURCE_DIR}/Editor/Terminal/SauceEngine.cpp
    ${CMAKE_SOURCE_DIR}/Editor/Shared/Shared.cpp
)

target_link_libraries(sauce-engine
    nlohmann_json::nlohmann_json
)

# UI Editor
add_executable(sauce-editor
    ${CMAKE_SOURCE_DIR}/Editor/UI/SauceEditor.cpp
    ${CMAKE_SOURCE_DIR}/Editor/Shared/Shared.cpp
)

target_link_libraries(sauce-editor PRIVATE
    glad
    glfw
    glm::glm
    nlohmann_json::nlohmann_json
    stb_image
    imgui
    sauce_opengl
    SauceEngine
    nativefiledialog
)

target_include_directories(sauce-editor PUBLIC
    ${CMAKE_SOURCE_DIR}/Engine
)
