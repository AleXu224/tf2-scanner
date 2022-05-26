#include "LoadingScreen.hpp"

#include "imgui.h"
#include "../globals.hpp"

using namespace ImGui;

void LoadingScreen() {
    if (!GLOBALS::scanner.showLoadingScreen) return;

    ImGuiIO& io = GetIO();
    ImDrawList* drawList = GetWindowDrawList();

    const float pwidth = 360.0f;
    const float pheight = 51.0f;

    PushFont(GLOBALS::FONTS[ROBOTO_16]);

    ImVec2 textSize = CalcTextSize("Loading...");
    ImVec2 textPos = {pwidth / 2 - textSize.x / 2, pheight / 2 - textSize.y / 2};
    ImVec2 childPos = {io.DisplaySize.x / 2 - pwidth / 2, io.DisplaySize.y / 2 - pheight / 2};

    PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.5f));
    PushStyleColor(ImGuiCol_ChildBg, COLORS::PRIMARY);
    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

    SetNextWindowPos({0, 0});
    SetNextWindowSize({io.DisplaySize.x, io.DisplaySize.y});

    Begin("LoadingScreen", nullptr, FLAGS - ImGuiWindowFlags_NoFocusOnAppearing);

    SetCursorPos(childPos);
    BeginChild("LoadingScreen", {pwidth, pheight});

    SetCursorPos(textPos);
    Text("Loading...");

    EndChild();

    End();

    PopStyleColor(2);
    PopStyleVar(1);
    PopFont();
}