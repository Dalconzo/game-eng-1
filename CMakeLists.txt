cmake_minimum_required(VERSION 3.14)
project(GameEngine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Engine library
add_library(engine
    src/core/engine.cpp
)

target_include_directories(engine 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Main executable
add_executable(game_app src/main.cpp)
target_link_libraries(game_app PRIVATE engine)
