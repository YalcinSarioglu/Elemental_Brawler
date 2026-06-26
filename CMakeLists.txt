cmake_minimum_required(VERSION 3.14)
project(ElementalBrawler)

if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()
set(CMAKE_POLICY_VERSION_MINIMUM "3.5" CACHE STRING "")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
  raylib
  URL https://github.com/raysan5/raylib/archive/refs/tags/5.0.tar.gz
)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)

add_executable(ElementalBrawler 
    src/main.cpp 
    src/entities/Entity.cpp 
    src/core/GameManager.cpp 
    src/entities/FighterBall.cpp
)
target_include_directories(ElementalBrawler PRIVATE src)
target_link_libraries(ElementalBrawler PRIVATE raylib)
