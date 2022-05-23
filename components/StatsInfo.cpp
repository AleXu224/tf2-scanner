#include "StatsInfo.hpp"

#include "../globals.hpp"
#include "imgui.h"
#include "string"

using namespace ImGui;

void StatsInfo(const bool *showWindow) {
    ImGuiIO &io = ImGui::GetIO();
    if (!*showWindow) return;
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
    const std::string show_window = "Show Window: " + std::to_string(*showWindow);
    Text("%s", show_window.c_str());
    const std::string show_drawer = "Show Drawer: " + std::to_string(GLOBALS::scanner.showDrawer);
    Text("%s", show_drawer.c_str());
    End();

    PopStyleColor();
}
