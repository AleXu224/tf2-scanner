cmake_minimum_required(VERSION 3.22)
project(bpscanner)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_RELEASE "-Os -DNDEBUG")
# set(CMAKE_CXX_FLAGS "-static -static-libgcc -static-libstdc++")
set(BUILD_SHARED_LIBS OFF)

# Imgui
file(GLOB imgui_files "imgui/*.cpp")
file(GLOB imgui_misc_files "imgui/misc/cpp/*.cpp")
file(GLOB imgui_glfw_files "imgui/backends/imgui_impl_glfw.cpp")
file(GLOB imgui_opengl3_files "imgui/backends/imgui_impl_opengl3.cpp")
add_library(imgui ${imgui_files} ${imgui_glfw_files} ${imgui_opengl3_files} ${imgui_misc_files})
target_include_directories(imgui PUBLIC imgui)
target_include_directories(imgui PUBLIC imgui/backends)
target_include_directories(imgui PUBLIC imgui/misc/cpp)

# GLFW
add_subdirectory(glfw)
include_directories(glfw/include)

# JSON
add_subdirectory(json)
include_directories(json/include)

# CPR
#add_subdirectory(cpr)
#include_directories(cpr/include)
include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG ee7f91f5566b6279aadf6d0b1aaf56b11963f527) # The commit hash for 1.8.2. Replace with the latest from: https://github.com/libcpr/cpr/releases
FetchContent_MakeAvailable(cpr)

add_executable(
        bpscanner 
        main.cpp 
        fonts/Roboto.cpp 
        fonts/MaterialIcons.cpp 
        components/TopBar.hpp 
        components/TopBar.cpp 
        components/StatsInfo.cpp 
        components/StatsInfo.hpp 
        globals.hpp 
        components/MainBody.cpp 
        components/MainBody.hpp 
        globals.cpp 
        components/SideBar.cpp 
        components/SideBar.hpp 
        scanning/Scanner.cpp 
        scanning/Scanner.hpp 
        scanning/Config.cpp 
        scanning/Config.hpp 
        components/Console.cpp 
        components/Console.hpp 
        json_schemas/Schema.hpp 
        json_schemas/Prices.hpp 
        json_schemas/Skins.hpp 
        json_schemas/Effects.hpp 
        json_schemas/UserGames.hpp
        json_schemas/UserLevel.hpp
        json_schemas/GithubVersion.hpp
        json_schemas/MarketPrices.hpp
        scanning/Player.hpp 
        scanning/Player.cpp 
        scanning/Inventory.hpp 
        scanning/Inventory.cpp 
        scanning/Item.hpp 
        scanning/Item.cpp 
        json_schemas/PlayerData.hpp 
        json_schemas/InventoryData.hpp
        utilities/stb_image.h
        utilities/Texture.hpp
        utilities/Texture.cpp
        utilities/stb_image_resize.h
        utilities/Animation.hpp
        utilities/Animation.cpp
        components/ApiKeyPrompt.hpp
        components/ApiKeyPrompt.cpp
        components/LoadingScreen.hpp
        components/LoadingScreen.cpp
        components/Overlay.hpp
        components/Overlay.cpp)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

target_link_libraries(bpscanner PRIVATE ${OPENGL_LIBRARIES})
target_link_libraries(bpscanner PRIVATE imgui)
target_link_libraries(bpscanner PRIVATE glfw)
target_link_libraries(bpscanner PRIVATE nlohmann_json)
target_link_libraries(bpscanner PRIVATE cpr::cpr)