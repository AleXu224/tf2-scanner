#include "SideBar.hpp"
#include "../globals.hpp"
#include "imgui.h"
#include "string"

using namespace ImGui;

void CustomIntInput(std::string input_name, int &input_value) {
    PushFont(GLOBALS::FONTS[ROBOTO_16]);

    SetCursorPosX(GetCursorPosX() + 16);
    SetCursorPosY(GetCursorPosY() + 16);
    Text("%s", input_name.c_str());
    SetCursorPosX(GetCursorPosX() + 16);
    ImGui::InputInt(input_name.c_str(), &input_value, 0, 0);

    PopFont();
}

void CustomFloatInput(std::string input_name, float &input_value) {
    PushFont(GLOBALS::FONTS[ROBOTO_16]);

    SetCursorPosX(GetCursorPosX() + 16);
    SetCursorPosY(GetCursorPosY() + 16);
    Text("%s", input_name.c_str());
    SetCursorPosX(GetCursorPosX() + 16);
    ImGui::InputFloat(input_name.c_str(), &input_value, 0, 0);

    PopFont();
}

void SideBarMenu() {
    const int SidebarWidth = 360;
    ImGuiIO &io = ImGui::GetIO();
    PushStyleColor(ImGuiCol_ChildBg, COLORS::PRIMARY);

    BeginChild("SideBar", ImVec2(SidebarWidth, GetWindowSize().y), false, FLAGS);

    CustomFloatInput("Max Refined", GLOBALS::scanner.config.maxRef);
    CustomFloatInput("Max Keys", GLOBALS::scanner.config.maxKeys);
    CustomFloatInput("Min Refined", GLOBALS::scanner.config.minRef);
    CustomFloatInput("Min Keys", GLOBALS::scanner.config.minKeys);
    CustomIntInput("Max Hours", GLOBALS::scanner.config.maxHours);
    CustomIntInput("Max history", GLOBALS::scanner.config.maxHistory);

    SetCursorPos(ImVec2(GetCursorPosX() + 16, GetCursorPosY() + 16));
    InputText("API Key", GLOBALS::scanner.config.apikey, 32);

    SetCursorPos(ImVec2(GetCursorPosX() + 16, GetCursorPosY() + 16));
    Checkbox("Untradable", &GLOBALS::scanner.config.untradable);
    SetCursorPos(ImVec2(GetCursorPosX() + 16, GetCursorPosY() + 16));
    Checkbox("No Value", &GLOBALS::scanner.config.noValue);
    SetCursorPos(ImVec2(GetCursorPosX() + 16, GetCursorPosY() + 16));
    Checkbox("Skins", &GLOBALS::scanner.config.skins);

    EndChild();

    PopStyleColor();
}

void SideBar() {
    static float progress = 0.0f;
    ImGuiIO &io = ImGui::GetIO();
    if (GLOBALS::scanner.showDrawer) {
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
        GLOBALS::scanner.showDrawer = false;
    }
    SameLine();
    SideBarMenu();
    End();

    PopStyleColor(1);
    PopStyleVar(2);
}