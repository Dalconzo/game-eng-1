cmake_minimum_required(VERSION 3.14)
project(GameEngine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find GLFW
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

# Find OpenGL
find_package(OpenGL REQUIRED)

# Handle GLEW based on platform
if(APPLE)
    # For macOS, use the Homebrew or system-installed GLEW
    find_package(GLEW REQUIRED)
else()
    # For other platforms, fetch GLEW if needed
    find_package(GLEW QUIET)
    if(NOT GLEW_FOUND)
        include(FetchContent)
        FetchContent_Declare(
            glew
            GIT_REPOSITORY https://github.com/nigels-com/glew.git
            GIT_TAG glew-2.2.0
        )
        FetchContent_MakeAvailable(glew)
    endif()
endif()
# Engine library
add_library(engine
    src/core/engine.cpp
    src/core/time_manager.cpp
    src/core/game_loop.cpp
    src/rendering/window.cpp
    src/rendering/shader.cpp
    src/rendering/mesh.cpp
    src/rendering/primitive_builder.cpp
    src/rendering/texture.cpp
    src/core/resource_manager.cpp
)
target_include_directories(engine 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
# Find or fetch GLM for math operations
find_package(glm QUIET)
if(NOT glm_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 0.9.9.8
    )
    FetchContent_MakeAvailable(glm)
endif()

# Update engine target - GLM is header-only so we include it, not link it
target_include_directories(engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/build/external
        ${GLM_INCLUDE_DIRS}
)

target_link_libraries(engine
    PUBLIC
        glfw
        OpenGL::GL
        GLEW::GLEW
)
# Main executable
add_executable(game_app src/main.cpp)
target_link_libraries(game_app PRIVATE engine)

set(CMAKE_TOOLCHAIN_FILE ~/development/tools/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")

# After the project() line
if(APPLE)
    add_definitions(-DGL_SILENCE_DEPRECATION)
endif()
