#include "StatsInfo.hpp"

#include "../globals.hpp"
#include "../utilities/Texture.hpp"
#include "imgui.h"
#include "string"

using namespace ImGui;

void StatsInfo::draw() {
    if (!GLOBALS::scanner.showStats) return;
    ImGuiIO &io = ImGui::GetIO();
    PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.5f));
    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImVec2 work_pos = viewport->WorkPos;
    ImVec2 work_size = viewport->WorkSize;
    SetNextWindowPos(ImVec2(25, work_pos.y + work_size.y - 25), ImGuiCond_Always, ImVec2(0, 1));
    SetNextWindowSize(ImVec2(0, 0));
    Begin("stats window", nullptr, (FLAGS | ImGuiWindowFlags_NoInputs) - ImGuiWindowFlags_NoFocusOnAppearing);

    const std::string fps = "FPS: " + std::to_string((int)io.Framerate);
    Text("%s", fps.c_str());
    const std::string frame_time = "Frame time: " + std::to_string(io.DeltaTime * 1000) + " ms";
    Text("%s", frame_time.c_str());
    const std::string is_scanning = "Is scanning: " + std::to_string(GLOBALS::scanner.isScanning);
    Text("%s", is_scanning.c_str());
    const std::string show_window = "Show Window: " + std::to_string(GLOBALS::scanner.showStats);
    Text("%s", show_window.c_str());
    const std::string show_drawer = "Show Drawer: " + std::to_string(GLOBALS::scanner.showDrawer);
    Text("%s", show_drawer.c_str());
    const std::string textures = "Textures: " + std::to_string(Texture::textures.size());
    Text("%s", textures.c_str());
    const std::string players = "Players: " + std::to_string(GLOBALS::scanner.playerList.size());
    Text("%s", players.c_str());
    int count = 0;
    for (auto &player : GLOBALS::scanner.playerList) {
        count += player.inventory.items.size();
    }
    const std::string items = "Total Items: " + std::to_string(count);
    Text("%s", items.c_str());
    End();

    PopStyleColor();
}
