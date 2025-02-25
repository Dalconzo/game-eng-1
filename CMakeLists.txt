cmake_minimum_required(VERSION 3.14)
project(GameEngine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Find the GLFW package
find_package(glfw3 REQUIRED)

# Find GLFW or build it from source
find_package(glfw3 QUIET)
if(NOT glfw3_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.3.8
    )
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(glfw)
endif()

# Engine library
add_library(engine
    src/core/engine.cpp
    src/rendering/window.cpp
)

if(TARGET glfw)
    get_target_property(GLFW_INCLUDE_DIRS glfw INTERFACE_INCLUDE_DIRECTORIES)
elseif(TARGET glfw3)
    get_target_property(GLFW_INCLUDE_DIRS glfw3 INTERFACE_INCLUDE_DIRECTORIES)
endif()

target_include_directories(engine 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${GLFW_INCLUDE_DIRS}
)
target_link_libraries(engine
    PUBLIC
        glfw
)

# Main executable
add_executable(game_app src/main.cpp)
target_link_libraries(game_app PRIVATE engine)

set(CMAKE_TOOLCHAIN_FILE ~/development/tools/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")
