//
// Created by Squizell on 27/04/2022.
//

#include <string>
#include "StatsInfo.hpp"
#include "imgui.h"
#include "../globals.hpp"

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
    Text(("FPS: " + std::to_string(io.Framerate)).c_str());
    Text(("Frame time: " + std::to_string(io.DeltaTime * 1000) + " ms").c_str());
    Text(("Show Window: " + std::to_string(*showWindow)).c_str());
    End();

    PopStyleColor();
}
