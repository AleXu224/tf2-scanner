cmake_minimum_required(VERSION 3.22)
project(bpscanner)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_RELEASE "-Os -DNDEBUG")
# set(CMAKE_CXX_FLAGS "-static -static-libgcc -static-libstdc++")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

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
        components/Overlay.cpp
        components/InfoCard.hpp
        components/InfoCard.cpp
)
        
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(cpr CONFIG REQUIRED)
find_package(Boost REQUIRED)

target_link_libraries(bpscanner PRIVATE ${OPENGL_LIBRARIES})
target_link_libraries(bpscanner PRIVATE imgui::imgui)
target_link_libraries(bpscanner PRIVATE glfw)
target_link_libraries(bpscanner PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(bpscanner PRIVATE cpr::cpr)
target_link_libraries(bpscanner PRIVATE Boost::boost)
        