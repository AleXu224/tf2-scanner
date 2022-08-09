#include "LoadingScreen.hpp"

#include "imgui.h"
#include "../globals.hpp"

using namespace ImGui;

void LoadingScreen::draw() {
    if (!shouldClose) {
        transition.forward();
    } else {
        transition.backward();
    }

    if (transition.getProgress() == 0.0f) canClose = true;
    OverlayBackground(transition);

    ImGuiIO& io = GetIO();

    const float pwidth = 360.0f;
    const float pheight = 51.0f;

    PushFont(GLOBALS::FONTS[ROBOTO_16]);

    ImVec2 textSize = CalcTextSize("Loading...");
    ImVec2 textPos = {pwidth / 2 - textSize.x / 2, pheight / 2 - textSize.y / 2};
    ImVec2 windowPos = {io.DisplaySize.x / 2 - pwidth / 2, io.DisplaySize.y / 2 - pheight / 2};
    windowPos.y += (1.0f - transition.getProgress()) * (io.DisplaySize.y - windowPos.y);

    PushStyleColor(ImGuiCol_WindowBg, COLORS::PRIMARY);
    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    PushStyleVar(ImGuiStyleVar_WindowRounding, 4.0f);

    SetNextWindowSize({io.DisplaySize.x, io.DisplaySize.y});

    SetNextWindowPos(windowPos);
    SetNextWindowSize({pwidth, pheight});

    Begin("LoadingScreen", nullptr, FLAGS);

    SetCursorPos(textPos);
    Text("Loading...");

    End();

    PopStyleVar(2);
    PopStyleColor(1);
    PopFont();
}