cmake_minimum_required(VERSION 3.10)
project(ecs_game)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ecs/include)

add_executable(game ecs/examples/main.cpp ecs/src/ECSManager.cpp)
target_link_libraries(game glfw GLEW::GLEW ${OPENGL_LIBRARIES})