cmake_minimum_required(VERSION 3.15)

if(NOT DEFINED CMAKE_GENERATOR AND NOT DEFINED ENV{CMAKE_GENERATOR})
    set(CMAKE_GENERATOR "Ninja" CACHE INTERNAL "Default generator")
endif()

project(cMaze LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE)
endif()

add_executable(cMaze
    src/main.cc
    src/classes/screen/screen.cc
    src/classes/entity/entity.cc
    src/classes/map/map.cc)

target_include_directories(cMaze PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_link_options(cMaze PRIVATE "-static")
endif()