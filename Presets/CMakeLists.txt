cmake_minimum_required(VERSION 3.20)

set(PROJECT "default-project")
set(LIB_PATH "/usr/lib/libSauceEngine.so")
set(INCLUDE_PATH "/usr/include/SauceEngine")

project(${PROJECT})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Executable Game

add_executable(${PROJECT}

    ${CMAKE_SOURCE_DIR}/Code/Main.cpp
)
target_include_directories(${PROJECT} PRIVATE
    ${CMAKE_SOURCE_DIR}/Code
    ${INCLUDE_PATH}
)

target_link_libraries(${PROJECT} PRIVATE 
    ${LIB_PATH}
)
