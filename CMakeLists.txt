cmake_minimum_required(VERSION 3.22)
project(bpscanner)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_RELEASE "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS "-static -static-libgcc -static-libstdc++")
set(VCPKG_FEATURE_FLAGS "versions")

# Imgui
file(GLOB imgui_files "imgui/*.cpp")
file(GLOB imgui_glfw_files "imgui/backends/imgui_impl_glfw.cpp")
file(GLOB imgui_opengl3_files "imgui/backends/imgui_impl_opengl3.cpp")
add_library(imgui ${imgui_files} ${imgui_glfw_files} ${imgui_opengl3_files})
target_include_directories(imgui PUBLIC imgui)
target_include_directories(imgui PUBLIC imgui/backends)

# GLFW
add_subdirectory(glfw)
include_directories(glfw/include)

add_executable(bpscanner main.cpp)

target_link_libraries(bpscanner PRIVATE imgui)
target_link_libraries(bpscanner PRIVATE glfw)
