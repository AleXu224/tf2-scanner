#include "Overlay.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "../globals.hpp"

using namespace ImGui;

void StatusIndicator::draw() {
    if (!GLOBALS::scanner.isScanning) return;
    ImGuiIO &io = GetIO();
    PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    SetNextWindowPos({0, 0});
    SetNextWindowSize({io.DisplaySize.x, io.DisplaySize.y});
    Begin("StatusIndicator", nullptr, FLAGS | ImGuiWindowFlags_NoInputs);
    PushFont(GLOBALS::FONTS[ROBOTO_12]);
    std::string &status = GLOBALS::scanner.scanStatus;
    ImVec2 textSize = CalcTextSize(status.c_str(), nullptr, true);
    const int height = 32;
    const int width = textSize.x + 16;
    const int margin = 16;
    ImVec2 pos = {io.DisplaySize.x - width - margin, io.DisplaySize.y - height - margin};
    ImVec2 pos2 = {io.DisplaySize.x - margin, io.DisplaySize.y - margin};
    ImVec2 textPos = {pos.x + 8, pos.y + 8};

    ImDrawList *draw_list = GetWindowDrawList();
    draw_list->AddRectFilled(pos, pos2, GetColorU32(COLORS::PRIMARY_LIGHT), 4.0f);
    draw_list->AddText(textPos, GetColorU32(COLORS::TEXT), status.c_str());
    PopFont();
    End();

    PopStyleColor();
}

bool OverlayBackground(Transition& transition) {
    SetNextWindowPos({0, 0});
    ImGuiIO& io = GetIO();
    SetNextWindowSize({io.DisplaySize.x, io.DisplaySize.y});
    PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.5f * transition.getProgress()));
    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    Begin("OverlayBG", nullptr, FLAGS);
    bool retVal = false;
    if (InvisibleButton("OverlayButton", {io.DisplaySize.x, io.DisplaySize.y})) {
        retVal = true;
    }
    End();

    PopStyleVar(1);
    PopStyleColor(1);

    return retVal;
}

bool OverlayBackground() {
    SetNextWindowPos({0, 0});
    ImGuiIO& io = GetIO();
    SetNextWindowSize({io.DisplaySize.x, io.DisplaySize.y});
    PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.5f));
    Begin("OverlayBG", nullptr, FLAGS);
    bool retVal = false;
    if (InvisibleButton("OverlayButton", {io.DisplaySize.x, io.DisplaySize.y})) {
        retVal = true;
    }
    End();

    return retVal;
}

void Overlay::addOverlay(OverlayBase *overlay) {
    overlays.push_back(overlay);
}

void Overlay::cleanup() {
    // Remove overlays that should and can be closed
    for (int i = 0; i < overlays.size();) {
        if (overlays[i]->shouldClose && overlays[i]->canClose) {
            delete overlays[i];
            overlays.erase(overlays.begin() + i);
            continue;
        }
        i++;
    }
}

void Overlay::draw() {
    cleanup();
    for (auto overlay : overlays) {
        overlay->draw();
    }
}

void Overlay::pop() {
    if (overlays.size() == 0) return;
    delete overlays.back();
    overlays.pop_back();
}
