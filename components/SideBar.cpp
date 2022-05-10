#include "SideBar.hpp"
#include "../globals.hpp"
#include "imgui.h"
#include "string"

using namespace ImGui;

void CustomTextInput(std::string input_name) { // NOLINT(performance-unnecessary-value-param)
    PushFont(GLOBALS::FONTS[ROBOTO_16]);

    SetCursorPosX(GetCursorPosX() + 16);
    SetCursorPosY(GetCursorPosY() + 16);
    Text("%s", input_name.c_str());
    SetCursorPosX(GetCursorPosX() + 16);
//    static char buf1[64] = ""; ImGui::InputText("default",     buf1, 64);

    PopFont();
}

void SideBarMenu() {
    const int SidebarWidth = 360;
    ImGuiIO &io = ImGui::GetIO();
    PushStyleColor(ImGuiCol_ChildBg, COLORS::PRIMARY);

    BeginChild("SideBar", ImVec2(SidebarWidth, GetWindowSize().y), false, FLAGS);

    CustomTextInput("Battan");

    EndChild();

    PopStyleColor();
}

void SideBar(bool &show) {
    static float progress = 0.0f;
    ImGuiIO &io = ImGui::GetIO();
    if (show) {
        progress += io.DeltaTime * (1.0f / 0.2f);
        if (progress > 1.0f) {
            progress = 1.0f;
        }
    } else {
        progress -= io.DeltaTime * (1.0f / 0.2f);
        if (progress < 0.0f) {
            progress = 0.0f;
        }
    }
    if (progress == 0.0f) return;
    const int SidebarWidth = 360;
    PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0.5f * progress));
    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y));
    SetNextWindowPos(ImVec2(0, 0));

    Begin("SideBar", nullptr, (FLAGS | ImGuiWindowFlags_NoScrollWithMouse) - ImGuiWindowFlags_NoFocusOnAppearing);
    if (InvisibleButton("SideBarBg", ImVec2(io.DisplaySize.x - SidebarWidth * BezierBlend(progress), io.DisplaySize.y))) {
        show = false;
    }
    SameLine();
    SideBarMenu();
    End();

    PopStyleColor(1);
    PopStyleVar(2);
}