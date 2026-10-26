cmake_minimum_required(VERSION 3.20)

set(PROJECT "default-project")
set(LIB_PATH "/usr/lib/libSauceEngine.so")
set(INCLUDE_PATH "/usr/include/SauceEngine")

project(${PROJECT})

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# GAME MODULES
#add_library(Game SHARED
#    ${CMAKE_SOURCE_DIR}/Code/Player.cpp
#)
#
#target_include_directories(Game PUBLIC
#    ${CMAKE_SOURCE_DIR}/Code
#    ${INCLUDE_PATH}
#    ${INCLUDE_PATH}/vendor
#)
#
#target_link_libraries(Game PUBLIC
#    ${LIB_PATH}
#)

# Executable Game

add_executable(${PROJECT}

    ${CMAKE_SOURCE_DIR}/Code/Main.cpp
)
target_include_directories(${PROJECT} PRIVATE
    ${CMAKE_SOURCE_DIR}/Code
    ${INCLUDE_PATH}
    ${INCLUDE_PATH}/vendor
)

target_link_libraries(${PROJECT} PRIVATE 
    ${LIB_PATH}
)
