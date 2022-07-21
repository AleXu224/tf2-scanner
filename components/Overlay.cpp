#include "Overlay.hpp"
#include "imgui.h"
#include "../globals.hpp"

using namespace ImGui;

void Overlay() {
    if (!GLOBALS::scanner.isScanning) return;
    ImGuiIO& io = GetIO();
    PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    SetNextWindowPos({0, 0});
    SetNextWindowSize({io.DisplaySize.x, io.DisplaySize.y});


    Begin("Overlay", nullptr, (FLAGS - ImGuiWindowFlags_NoFocusOnAppearing) | ImGuiWindowFlags_NoInputs);
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