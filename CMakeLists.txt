cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")

project(Sorin_Engine)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(assimp)
add_subdirectory(imgui)
add_subdirectory(editor)
add_subdirectory(engine)
add_subdirectory(glfw)


